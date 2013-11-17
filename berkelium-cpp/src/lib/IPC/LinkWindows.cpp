// Copyright (c) 2012 The Berkelium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifdef OS_WINDOWS

#include <Windows.h>

#include <Berkelium/API/BerkeliumFactory.hpp>
#include <Berkelium/API/Logger.hpp>
#include <Berkelium/IPC/Message.hpp>
#include <Berkelium/IPC/Link.hpp>
#include <Berkelium/IPC/LinkGroup.hpp>
#include <Berkelium/Impl/Filesystem.hpp>
#include <Berkelium/Impl/Impl.hpp>

#include <sstream>

#ifdef BERKELIUM_TRACE_IO_DATA
#define trace 1
#else
#define trace 0
#endif

using Berkelium::impl::Filesystem;

namespace Berkelium {

namespace Ipc {

namespace impl {

class LinkWindowsImpl : public Link {
private:
	static const int pipeInstances = 1;
	static const int pipeTimeout = 5000;
	static const int bufferSize = 4096;

	LoggerRef logger;
	LinkGroupRef group;
	const std::string name;
	const std::string full;
	const std::string alias;
	bool server;
	HANDLE pipe;

public:
	LinkWindowsImpl(bool server, LinkGroupRef group, LoggerRef logger, const std::string& dir, const std::string& name, const std::string& alias) :
		Link(),
		logger(logger),
		group(group),
		name(name),
		full("\\\\.\\pipe\\" + name),
		alias(alias),
		server(server),
		pipe(INVALID_HANDLE_VALUE) {
		TRACE_OBJECT_NEW("LinkWindowsImpl");

		if (server) {
			pipe = CreateNamedPipe(full.c_str(), PIPE_ACCESS_DUPLEX | FILE_FLAG_OVERLAPPED, PIPE_TYPE_BYTE | PIPE_WAIT, 
				pipeInstances, bufferSize * sizeof(TCHAR), bufferSize * sizeof(TCHAR), pipeTimeout, NULL);
			if (pipe == INVALID_HANDLE_VALUE) {
				logger->systemError("CreateNamedPipe", full);
				return;
			}
			
			OVERLAPPED connectOverlap;
			connectOverlap.hEvent = CreateEvent(NULL, TRUE, TRUE, NULL);
			BOOL error = ConnectNamedPipe(pipe, &connectOverlap);
			if (error) {
				logger->systemError("ConnectNamedPipe", full);
				CloseHandle(pipe);
				return;
			}
		} else {
			pipe = CreateFile(full.c_str(), GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 
				FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED, NULL);
			if (pipe == INVALID_HANDLE_VALUE) {
				printf("CreateNamedPipe failed with %d.\n", GetLastError());
				logger->systemError("CreateFile", full);
				return;
			}
		}
	}

	virtual ~LinkWindowsImpl() {
		TRACE_OBJECT_DELETE("LinkWindowsImpl");
		if (pipe == INVALID_HANDLE_VALUE) {
			CloseHandle(pipe);
		}
	}

	virtual bool isEmpty() {
		if (pipe == INVALID_HANDLE_VALUE) {
			return true;
		}

		DWORD bytesAvail = 0;
		PeekNamedPipe(pipe, NULL, NULL, NULL, &bytesAvail, NULL);
		return bytesAvail == 0;
	}

	virtual void send(MessageRef msg) {
		if (pipe == INVALID_HANDLE_VALUE) {
			return;
		}

		int32_t size = msg->data_length();
		WriteFile(pipe, &size, 4, NULL, NULL);
		WriteFile(pipe, msg->data(), size, NULL, NULL);
		msg->reset();
	}

	virtual MessageRef recv() {
		int32_t size = 0;
		recv((char*)&size, 4);
		return recv(size);
	}

	virtual MessageRef recv(int32_t size) {
		MessageRef msg(Message::create(logger));
		msg->setup(size - sizeof(int32_t));
		recv((char*)msg->data(), size);
		return msg;
	}

	void recv(char* to, size_t size) {
		ReadFile(pipe, to, size, NULL, NULL);
	}

	virtual const std::string getName() {
		return name;
	}

	virtual const std::string getAlias() {
		return alias;
	}

	HANDLE getLinkFd() {
		return pipe;
	}
};

} // namespace impl

Link::Link() {
}

Link::~Link() {
}

LinkRef Link::getLink(bool server, LinkGroupRef group, LoggerRef logger, const std::string& dir, const std::string& name, const std::string& alias) {
	LinkRef ret(new impl::LinkWindowsImpl(server, group, logger, dir, name, alias));
	if (group) {
		group->registerLink(ret);
	}
	return ret;
}

} // namespace Ipc

namespace impl {

Berkelium::Ipc::LinkFdType getLinkFd(Ipc::LinkRef pipe) {
	if (!pipe) {
		Berkelium::impl::bk_error("getLinkFd: pipe is NULL!");
		return 0;
	}
	return ((Ipc::impl::LinkWindowsImpl*)pipe.get())->getLinkFd();
}

} // namespace Ipc

} // namespace Berkelium

#endif // OS_WINDOWS
