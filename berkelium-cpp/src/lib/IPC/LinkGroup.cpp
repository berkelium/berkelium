// Copyright (c) 2012 The Berkelium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Berkelium/API/Util.hpp>
#include <Berkelium/IPC/LinkGroup.hpp>
#include <Berkelium/IPC/Ipc.hpp>
#include <Berkelium/IPC/Link.hpp>
#include <Berkelium/Impl/Impl.hpp>

#include <map>

#ifdef WINDOWS
#include <set>
#include <windows.h> 
#endif

using Berkelium::impl::getLinkFd;
using Berkelium::impl::bk_error;

#ifdef BERKELIUM_TRACE_IO_SELECT
#define trace 1
#else
#define trace 0
#endif

namespace Berkelium {

namespace Ipc {

LinkCallback::LinkCallback() {
	TRACE_OBJECT_NEW("LinkCallback");
}

LinkCallback::~LinkCallback() {
	TRACE_OBJECT_DELETE("LinkCallback");
}

namespace impl {

#define BUFFER_SIZE 4096

struct LinkGroupData {
	LinkWRef ref;
	LinkCallbackWRef cb;
	Berkelium::Ipc::LinkFdType fd;
	uint8_t buffer[BUFFER_SIZE];
#ifdef WINDOWS
	DWORD size;
	bool pending;
	OVERLAPPED overlapped;
#elif LINUX
	int32_t size;
#endif
};

typedef std::map<Link*, LinkGroupData*> LinkMap;

class LinkGroupImpl : public LinkGroup {
private:
	int64_t lastRecv;
	LinkMap map;
#ifdef WINDOWS
	std::set<HANDLE> events;
	std::set<LinkGroupData*> linkGroupDatas;
#endif
#ifdef LINUX
	fd_set fds;
#endif

public:
	LinkGroupImpl() :
		LinkGroup(),
		lastRecv(0),
		map() {
		TRACE_OBJECT_NEW("LinkGroupImpl");
	}

	virtual ~LinkGroupImpl() {
		TRACE_OBJECT_DELETE("LinkGroupImpl");
	}

	virtual void waitForInit() {
		for(LinkMap::iterator it(map.begin()); it != map.end(); it++) {
			LinkGroupData* data = it->second;
#ifdef WINDOWS
			if(data->fd == INVALID_HANDLE_VALUE) {
				continue;
			}
#elif LINUX
			if(data->fd == -1) {
				continue;
			}
#endif
			LinkRef ref(data->ref.lock());
			if(!ref) {
				continue;
			}
			ref->waitForInit();
		}
	}

	virtual void registerLink(LinkRef link) {
		LinkGroupData* data = new LinkGroupData();
		data->ref = link;
		data->fd = getLinkFd(link);
#ifdef WINDOWS
		data->size = 0;
		data->pending = false;
		data->overlapped.Offset = 0;
		data->overlapped.OffsetHigh = 0;
		data->overlapped.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
		events.insert(data->overlapped.hEvent);
		linkGroupDatas.insert(data);
#endif
		map.insert(std::pair<Link*, LinkGroupData*>(link.get(), data));
		//Berkelium::impl::bk_error("register link %p %s fd:%d", link.get(), link->getName().c_str(), data->fd);
	}

	virtual void unregisterLink(Link* link) {
		//Berkelium::impl::bk_error("unregister link %p", link);
		LinkMap::iterator it(map.find(link));
		if(it != map.end()) {
			LinkGroupData* data = it->second;
#ifdef WINDOWS
			CancelIoEx(data->fd, &data->overlapped);
			events.erase(data->overlapped.hEvent);
			linkGroupDatas.erase(data);
#endif
			delete data;
			map.erase(it);
		}
	}

	virtual void registerCallback(LinkRef link, LinkCallbackWRef callback) {
		//Berkelium::impl::bk_error("register callback %p", link.get());
		LinkMap::iterator it(map.find(link.get()));
		if(it != map.end()) {
			LinkGroupData* data = it->second;
#ifdef WINDOWS
			if(data->fd == INVALID_HANDLE_VALUE) {
				Berkelium::impl::bk_error("register callback %p failed: no fd!", link.get());
			}
#elif LINUX
			if(data->fd == -1) {
				Berkelium::impl::bk_error("register callback %p failed: no fd!", link.get());
			}
#endif
			data->cb = callback;
		}
		else {
			Berkelium::impl::bk_error("register callback %p failed!", link.get());
		}
	}

	virtual void registerCallback(ChannelGroupRef group, LinkCallbackWRef callback) {
		registerCallback(Berkelium::impl::getInputLink(group), callback);
	}

	virtual int64_t getLastRecv() {
		return lastRecv;
	}

	virtual void update(int32_t timeout) {
		if(timeout < 0) {
			recv((int32_t) -1);
			return;
		}
		int64_t end = Util::currentTimeMillis() + timeout;
		while(true) {
			int64_t now = Util::currentTimeMillis();
			int64_t left = end - now;
			if(left < 1) {
				return;
			}
			recv((int32_t) left);
		}
	}

	void recv(int32_t timeout) {
#ifdef WINDOWS
		LinkGroupData* readyData = NULL;
		std::vector<HANDLE> handles;
		for(LinkMap::iterator it(map.begin()); it != map.end(); it++) {
			LinkGroupData* data = it->second;
			if (data->fd == INVALID_HANDLE_VALUE) {
				continue;
			}
			if (data->pending) {
				handles.push_back(data->overlapped.hEvent);
				continue;
			}

			if(trace) {
				LinkRef ref(data->ref.lock());
				bk_error("LinkGroup: reading %d for %d - %d", ref->getAlias().c_str(), data->fd, ref->getName().c_str());
			}

			BOOL success = ReadFile(data->fd, &data->buffer, BUFFER_SIZE, &data->size, &data->overlapped);
			if (success && data->size != 0) {
				data->pending = false;
				readyData = data;
				continue;
			}

			if (!success && (GetLastError() == ERROR_IO_PENDING)) {
				data->pending = true;
				handles.push_back(data->overlapped.hEvent);
				continue;
			}
		}

		if (readyData == NULL) {
			if (handles.empty()) {
				bk_error("LinkGroup: cannot wait for events, list is empty...");
				return;
			}

			if(trace) {
				bk_error("LinkGroup: waiting for %d events...", handles.size());
			}
			DWORD result = WaitForMultipleObjects(handles.size(), &handles[0], FALSE, timeout == -1 ? INFINITE : timeout);
			int linkIndex = result - WAIT_OBJECT_0;
			if (linkIndex < 0 || linkIndex > (events.size() - 1)) {
				return;
			}
		
			std::set<LinkGroupData*>::iterator it = linkGroupDatas.begin();
			std::advance(it, linkIndex);
			readyData = *it;

			if (!GetOverlappedResult(readyData->fd, &readyData->overlapped, &readyData->size, TRUE)) {
				// TODO maybe handle Broken Pipe ?
				return;
			}
			
			ResetEvent(readyData->overlapped.hEvent);
			readyData->pending = false;
		}

		if (readyData->size == 0) {
			bk_error("LinkGroup: no data ready!");
			return;
		}

		lastRecv = Util::currentTimeMillis();

		LinkRef ref(readyData->ref.lock());
		if(trace) {
			bk_error("LinkGroup: selected %s %d %s", ref->getAlias().c_str(), readyData->fd, ref->getName().c_str());
		}
		LinkCallbackRef cb(readyData->cb.lock());
		if(cb) {
			cb->onLinkDataReady(ref, (uint32_t)readyData->size, readyData->buffer);
		} else {
			bk_error("LinkGroup: no callback!");
		}
#elif LINUX
		FD_ZERO(&fds);
		int nfds = 0;
		if(trace) {
			bk_error("LinkGroup: selecting...");
		}
		for(LinkMap::iterator it(map.begin()); it != map.end(); it++) {
			LinkGroupData* data = it->second;
			int fd = data->fd;
			if(fd == -1) {
				continue;
			}
			LinkRef ref(data->ref.lock());
			if(!ref) {
				continue;
			}
			nfds = std::max(nfds, fd);
			FD_SET(fd, &fds);
			if(trace) {
				//if(ref->getAlias().substr(0, 7) != "process")
				//if(ref->getAlias() != "berkelium-host-ipc")
				bk_error("LinkGroup: waiting for %s %d %s", ref->getAlias().c_str(), data->fd, ref->getName().c_str());
			}
		}
		static struct timeval tv;
		tv.tv_sec = 0;
		tv.tv_usec = timeout * 1000;
		int r = select(nfds + 1, &fds, NULL, NULL, timeout == -1 ? NULL : &tv);
		if(r == 0) {
			return;
		} else if(r == -1) {
			bk_error("LinkGroup: select error %d", r);
			return;
		}
		lastRecv = Util::currentTimeMillis();
		for(LinkMap::iterator it(map.begin()); it != map.end(); it++) {
			LinkGroupData* data = it->second;
			if(data->fd == -1) {
				continue;
			}
			if(FD_ISSET(data->fd, &fds)) {
				LinkRef ref(data->ref.lock());
				if(trace) {
					bk_error("LinkGroup: selected %s %d %s", ref->getAlias().c_str(), data->fd, ref->getName().c_str());
				}
				LinkCallbackRef cb(data->cb.lock());
				if(cb) {
					ssize_t size = ::read(data->fd, &data->buffer, BUFFER_SIZE);
					if(size == -1) {
						bk_error("read failed: %d", size);
					} else {
						// size can not be larger then BUFFER_SIZE,
						// because of this size can be cast to 32 bit even on 64 bit systems
						cb->onLinkDataReady(ref, (uint32_t)size, data->buffer);
					}
				} else {
					bk_error("LinkGroup: no callback!");
				}
			}
		}
#endif
	}
};

} // namespace impl

LinkGroup::LinkGroup() {
	TRACE_OBJECT_NEW("LinkGroup");
}

LinkGroup::~LinkGroup() {
	TRACE_OBJECT_DELETE("LinkGroup");
}

LinkGroupRef LinkGroup::create() {
	return LinkGroupRef(new impl::LinkGroupImpl());
}

} // namespace Ipc

} // namespace Berkelium
