// Copyright (c) 2012 The Berkelium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifdef OS_WINDOWS

#include <Berkelium/API/BerkeliumFactory.hpp>
#include <Berkelium/API/Logger.hpp>
#include <Berkelium/IPC/Message.hpp>
#include <Berkelium/IPC/Link.hpp>
#include <Berkelium/IPC/LinkGroup.hpp>
#include <Berkelium/Impl/Filesystem.hpp>
#include <Berkelium/Impl/Impl.hpp>

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
	LoggerRef logger;
	LinkGroupRef group;
	const std::string name;
	const std::string dir;
	const std::string full;
	const std::string alias;
	bool read;
	int fd;
	//fd_set fds;

public:
	LinkWindowsImpl(bool read, LinkGroupRef group, LoggerRef logger, const std::string& dir, const std::string& name, const std::string& alias) :
		Link(),
		logger(logger),
		group(group),
		name(name),
		dir(dir),
		full(Filesystem::append(dir, name)),
		alias(alias),
		read(read),
		fd(-1) {

		Filesystem::createDirectories(dir);

		/*const char* p = full.c_str();
		if(::access(p, F_OK) != 0 && ::mkfifo(p, 0700) != 0) {
			logger->systemError("mkfifo", full);
			return;
		}
		fd = ::open(p, O_RDWR);
		if(fd == -1) {
			logger->systemError("open", full);
			return;
		}*/

		//fprintf(stderr, "new Link %s %d %s\n", name.c_str(), fd, alias.c_str());
	}

	virtual ~LinkWindowsImpl() {
		/*if(group) {
			group->unregisterLink(this);
		}
		if(fd != -1) {
			close(fd);
			fd = -1;
		}*/
		Filesystem::removeFile(full);
	}

	virtual bool isEmpty() {
        // TODO
		return true;
	}

	virtual void send(MessageRef msg) {
        // TODO
	}

	virtual MessageRef recv() {
        // TODO
        return NULL;
	}

	void recv(char* to, size_t size) {
        // TODO
	}

	virtual const std::string getName() {
		return name;
	}

	virtual const std::string getAlias() {
		return alias;
	}

	int getLinkFd() {
		if(!read) {
			return -1;
		}
		return fd;
	}
};

} // namespace impl

Link::Link() {
}

Link::~Link() {
}

LinkRef Link::getLink(bool read, LinkGroupRef group, LoggerRef logger, const std::string& dir, const std::string& name, const std::string& alias) {
	LinkRef ret(new impl::LinkWindowsImpl(read, group, logger, dir, name, alias));
    if(read && group) {
		group->registerLink(ret);
	}
	return ret;
}

} // namespace Ipc

namespace impl {

int getLinkFd(Ipc::LinkRef pipe) {
	if(!pipe) {
		Berkelium::impl::bk_error("getLinkFd: pipe is NULL!");
		return 0;
	}
	return ((Ipc::impl::LinkWindowsImpl*)pipe.get())->getLinkFd();
}

} // namespace impl

} // namespace Berkelium

#endif // OS_WINDOWS
