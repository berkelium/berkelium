// Copyright (c) 2012 The Berkelium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Berkelium/API/Util.hpp>
#include <Berkelium/IPC/ChannelGroup.hpp>
#include <Berkelium/IPC/Ipc.hpp>
#include <Berkelium/IPC/Pipe.hpp>
#include <Berkelium/Impl/Impl.hpp>

#include <map>

using Berkelium::impl::getPipeFd;
using Berkelium::impl::bk_error;

namespace Berkelium {

namespace Ipc {

namespace impl {

struct ChannelGroupData {
	ChannelWRef ref;
	ChannelCallbackWRef cb;
	int fd;
	bool highPrio;
};

typedef std::map<Channel*, ChannelGroupData*> ChannelMap;

class ChannelGroupImpl : public ChannelGroup {
private:
	ChannelMap map;
	fd_set fds;

public:
	ChannelGroupImpl() :
		ChannelGroup(),
		map() {
	}

	virtual ~ChannelGroupImpl() {
	}

	virtual void registerChannel(ChannelRef channel) {
		//Berkelium::impl::bk_error("register channel %p", channel.get());
		ChannelGroupData* data = new ChannelGroupData();
		data->ref = channel;
		data->fd = getPipeFd(channel, true);
		map.insert(std::pair<Channel*, ChannelGroupData*>(channel.get(), data));
	}

	virtual void unregisterChannel(Channel* channel) {
		//Berkelium::impl::bk_error("unregister channel %p", channel);
		ChannelMap::iterator it(map.find(channel));
		if(it != map.end()) {
			ChannelGroupData* data = it->second;
			delete data;
			map.erase(it);
		}
	}

	virtual void registerCallback(ChannelRef channel, ChannelCallbackRef callback, bool highPrio) {
		//Berkelium::impl::bk_error("register callback %p", channel.get());
		ChannelMap::iterator it(map.find(channel.get()));
		if(it != map.end()) {
			ChannelGroupData* data = it->second;
			data->cb = callback;
			data->highPrio = highPrio;
		} else {
			Berkelium::impl::bk_error("register callback %p failed!", channel.get());
		}
	}

	virtual void update(int32_t timeout) {
		int64_t end = Util::currentTimeMillis() + timeout;
		//Berkelium::impl::bk_error("update %d", timeout);
		while(true) {
			int64_t now = Util::currentTimeMillis();
			int64_t left = end - now;
			//Berkelium::impl::bk_error("update now %ld end %ld left %ld", now, end, left);
			if(left < 1) {
				return;
			}
			recv(ChannelRef(), left);
		}
	}

	virtual void recv(ChannelRef channel, PipeRef pipe, MessageRef msg) {
		//Berkelium::impl::bk_error("recv wait %p", channel.get());
		while(channel->isEmpty()) {
			//Berkelium::impl::bk_error("recv update %p", channel.get());
			recv(channel, -1);
		}
		//Berkelium::impl::bk_error("recv read %p", channel.get());
		pipe->recv(msg);
		//Berkelium::impl::bk_error("recv done %p", channel.get());
	}

	void recv(ChannelRef channel, int32_t timeout) {
		FD_ZERO(&fds);
		int nfds = 0;
		bool all = !channel;
		for(ChannelMap::iterator it(map.begin()); it != map.end(); it++) {
			ChannelGroupData* data = it->second;
			ChannelRef ref(data->ref.lock());
			if(ref && (all || data->highPrio || ref.get() == channel.get())) {
				int fd = data->fd;
				nfds = std::max(nfds, fd);
				FD_SET(fd, &fds);
			}
		}
		static struct timeval tv;
		tv.tv_sec = 0;
		tv.tv_usec = timeout * 1000;
		int r = select(nfds + 1, &fds, NULL, NULL, timeout == -1 ? NULL : &tv);
		if(r == 0) {
			return;
		} else if(r == -1) {
			bk_error("ChannelGroup: select error %d", r);
			return;
		}
		for(ChannelMap::iterator it(map.begin()); it != map.end(); it++) {
			ChannelGroupData* data = it->second;
			if(FD_ISSET(data->fd, &fds)) {
				ChannelCallbackRef cb(data->cb.lock());
				if(cb) {
					cb->onDataReady(data->ref.lock());
				}
			}
		}
	}
};

} // namespace impl

ChannelGroup::ChannelGroup() {
}

ChannelGroup::~ChannelGroup() {
}

ChannelGroupRef ChannelGroup::create() {
	return ChannelGroupRef(new impl::ChannelGroupImpl());
}

} // namespace Ipc

} // namespace Berkelium
