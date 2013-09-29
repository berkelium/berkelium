// Copyright (c) 2012 The Berkelium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Berkelium/API/Util.hpp>
#include <Berkelium/API/Logger.hpp>
#include <Berkelium/IPC/Pipe.hpp>
#include <Berkelium/IPC/Message.hpp>
#include <Berkelium/IPC/ChannelGroup.hpp>
#include <Berkelium/Impl/Impl.hpp>
#include <Berkelium/Impl/Filesystem.hpp>

using Berkelium::impl::Filesystem;

namespace Berkelium {

namespace Ipc {

namespace impl {

class ChannelImpl : public Channel {
private:
	ChannelGroupRef group;
	LoggerRef logger;
	const std::string dir;
	const std::string name;
	const bool server;
	const bool reverse;
	ChannelRef reverseRef;
	ChannelWRef reverseWRef;
	ChannelWRef self;
	MessageRef message;
	PipeRef pin;
	PipeRef pout;

	static inline std::string getExt(const bool server, const bool reverse) {
		if(reverse) {
			return server ? "3" : "4";
		}
		return server ? "1" : "2";
	}

public:
	ChannelImpl(ChannelGroupRef group, LoggerRef logger, const std::string& dir, const std::string& name, const bool server, const bool reverse) :
		Channel(),
		group(group),
		logger(logger),
		dir(dir),
		name(name),
		server(server),
		reverse(reverse),
		reverseRef(),
		self(),
		message(Message::create(logger)),
		pin(Pipe::getPipe(logger, Filesystem::append(dir, name + getExt(server, reverse)))),
		pout(Pipe::getPipe(logger, Filesystem::append(dir, name + getExt(!server, reverse)))) {
	}

	virtual ~ChannelImpl() {
		group->unregisterChannel(this);
	}

	virtual bool isEmpty() {
		return pin->isEmpty();
	}

	virtual Ipc::MessageRef getMessage() {
		return message;
	}

	virtual void send(Ipc::MessageRef msg) {
		pout->send(msg);
	}

	virtual void recv(Ipc::MessageRef msg) {
		pin->recv(msg);
	}

	virtual ChannelRef createSubChannel() {
		return Channel::createChannel(group, logger, dir, true);
	}

	virtual ChannelRef getSubChannel(const std::string& name) {
		return Channel::getChannel(group, logger, dir, name, false);
	}

	virtual ChannelRef getReverseChannel() {
		if(reverseRef) {
			return reverseRef;
		}
		return reverseWRef.lock();
	}

	virtual std::string getName() {
		return name;
	}

	virtual ChannelGroupRef getGroup() {
		return group;
	}

	ChannelRef getSelf() {
		return self.lock();
	}

	static ChannelRef newChannel(ChannelGroupRef group, LoggerRef logger, const std::string& dir, const std::string& name, const bool server) {
		ChannelImpl* impl1 = new ChannelImpl(group, logger, dir, name, server, false);
		ChannelImpl* impl2 = new ChannelImpl(group, logger, dir, name, server, true);
		ChannelRef ret1(impl1);
		ChannelRef ret2(impl2);
		impl1->self = ret1;
		impl2->self = ret2;
		impl1->reverseRef = ret2;
		impl2->reverseWRef = ret1;
		group->registerChannel(ret1);
		group->registerChannel(ret2);
		return ret1;
	}

#ifdef LINUX
	int getPipeFd(bool in) {
		if(in) {
			return Berkelium::impl::getPipeFd(pin);
		} else {
			return Berkelium::impl::getPipeFd(pout);
		}
	}
#endif // LINUX
};

} // namespace impl

Channel::Channel() {
}

Channel::~Channel() {
}

ChannelRef Channel::createChannel(ChannelGroupRef group, LoggerRef logger, const std::string& dir, const bool server) {
	return getChannel(group, logger, dir, Util::randomId(), server);
}

ChannelRef Channel::getChannel(ChannelGroupRef group, LoggerRef logger, const std::string& dir, const std::string& name, const bool server) {
	return impl::ChannelImpl::newChannel(group, logger, dir, name, server);
}

} // namespace Ipc

#ifdef LINUX

namespace impl {

int getPipeFd(Ipc::ChannelRef channel, bool in)
{
       if(!channel) {
               Berkelium::impl::bk_error("getPipeFd: channel is NULL!");
               return 0;
       }
       return ((Ipc::impl::ChannelImpl*)channel.get())->getPipeFd(in);
}

} // namespace impl

#endif // LINUX

} // namespace Berkelium
