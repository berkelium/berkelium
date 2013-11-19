// Copyright (c) 2012 The Berkelium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifdef LINUX

#include <Berkelium/API/BerkeliumFactory.hpp>
#include <Berkelium/API/Logger.hpp>
#include <Berkelium/IPC/Message.hpp>
#include <Berkelium/IPC/Link.hpp>
#include <Berkelium/IPC/LinkGroup.hpp>
#include <Berkelium/IPC/PipePosix.hpp>
#include <Berkelium/Impl/Impl.hpp>

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sstream>

namespace Berkelium {

namespace Ipc {

namespace impl {

class LinkLinuxImpl : public Link {
private:
	LoggerRef logger;
	LinkGroupRef group;
	PipePosixRef pin;
	PipePosixRef pout;
	std::string name;
	std::string alias;

	static inline std::string getExt(const bool server) {
		return server ? "1" : "2";
	}

public:
	LinkLinuxImpl(bool server, LinkGroupRef group, LoggerRef logger, const std::string& dir, const std::string& name, const std::string& alias) :
		Link(),
		logger(logger),
		group(group),
		pin(PipePosix::create(server, true, logger, dir, name + getExt(server), alias + getExt(server) + "In")),
		pout(PipePosix::create(server, false, logger, dir, name + getExt(!server), alias + getExt(!server) + "Out")),
		name(name),
		alias(alias) {
		TRACE_OBJECT_NEW("LinkLinuxImpl");
	}

	virtual ~LinkLinuxImpl() {
		TRACE_OBJECT_DELETE("LinkLinuxImpl");
		if(group) {
			group->unregisterLink(this);
		}
	}

	bool isOk() {
		return pin && pout;
	}

	virtual bool isEmpty() {
		return pin->isEmpty();
	}

	virtual void waitForInit() {
	}

	virtual void send(MessageRef msg) {
		pout->send(msg);
	}

	virtual MessageRef recv() {
		return pin->recv();
	}

	virtual const std::string getName() {
		return name;
	}

	virtual const std::string getAlias() {
		return alias;
	}

	int getLinkFd() {
		return Berkelium::impl::getLinkFd(pin);
	}
};

} // namespace impl

Link::Link() {
}

Link::~Link() {
}

LinkRef Link::getLink(bool server, LinkGroupRef group, LoggerRef logger, const std::string& dir, const std::string& name, const std::string& alias) {
	impl::LinkLinuxImpl* impl(new impl::LinkLinuxImpl(server, group, logger, dir, name, alias));
	LinkRef ret(impl);
	if(!impl->isOk()) {
		return LinkRef();
	}
	if(group) {
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
	return ((Ipc::impl::LinkLinuxImpl*)pipe.get())->getLinkFd();
}

} // namespace impl

} // namespace Berkelium

#endif // LINUX
