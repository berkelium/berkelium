// Copyright (c) 2012 The Berkelium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Berkelium/API/Util.hpp>
#include <Berkelium/API/Logger.hpp>
#include <Berkelium/IPC/Pipe.hpp>
#include <Berkelium/IPC/Message.hpp>
#include <Berkelium/IPC/PipeGroup.hpp>
#include <Berkelium/Impl/Impl.hpp>
#include <Berkelium/Impl/Filesystem.hpp>

using Berkelium::impl::Filesystem;

namespace Berkelium {

namespace Ipc {

namespace impl {

class ChannelImpl : public Channel {
private:
	PipeGroupRef group;
	LoggerRef logger;
	const std::string dir;
	const std::string name;
	const std::string alias;
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
	ChannelImpl(PipeGroupRef group, LoggerRef logger, const std::string& dir, const std::string& name, const std::string& alias, const bool server, const bool reverse) :
		Channel(),
		group(group),
		logger(logger),
		dir(dir),
		name(name),
		alias(alias),
		server(server),
		reverse(reverse),
		reverseRef(),
		self(),
		message(Message::create(logger)),
		pin(Pipe::getPipe(true, group, logger, dir, name + getExt(server, reverse), alias)),
		pout(Pipe::getPipe(false, group, logger, dir, name + getExt(!server, reverse), alias)) {
	}

	virtual ~ChannelImpl() {
	}

	virtual Ipc::MessageRef getMessage() {
		return message;
	}

	virtual void send(Ipc::MessageRef msg) {
		pout->send(msg);
	}

	virtual void recv(Ipc::MessageRef msg) {
		group->recv(pin, msg, -1);
	}

	virtual bool recv(Ipc::MessageRef msg, int32_t timeout) {
		return group->recv(pin, msg, timeout);
	}

	virtual ChannelRef createSubChannel(const std::string& alias) {
		return Channel::createChannel(group, logger, dir, alias, true);
	}

	virtual ChannelRef getSubChannel(const std::string& name, const std::string& alias) {
		return Channel::getChannel(group, logger, dir, name, alias, false);
	}

	virtual ChannelRef getReverseChannel() {
		if(reverseRef) {
			return reverseRef;
		}
		return reverseWRef.lock();
	}

	virtual bool isReverseChannel() {
		return reverse;
	}

	virtual std::string toString() {
		return name + getExt(server, reverse) + " (" + alias + ")";
	}

	virtual std::string getName() {
		return name;
	}

	virtual std::string getAlias() {
		return alias;
	}

	virtual PipeGroupRef getGroup() {
		return group;
	}

	ChannelRef getSelf() {
		return self.lock();
	}

	static ChannelRef newChannel(PipeGroupRef group, LoggerRef logger, const std::string& dir, const std::string& name, const std::string& alias, const bool server) {
		ChannelImpl* impl1 = new ChannelImpl(group, logger, dir, name, alias, server, false);
		ChannelImpl* impl2 = new ChannelImpl(group, logger, dir, name, alias, server, true);
		ChannelRef ret1(impl1);
		ChannelRef ret2(impl2);
		impl1->self = ret1;
		impl2->self = ret2;
		impl1->reverseRef = ret2;
		impl2->reverseWRef = ret1;
		return ret1;
	}

	Ipc::PipeRef getInputPipe() {
		return pin;
	}
};

} // namespace impl

Channel::Channel() {
}

Channel::~Channel() {
}

ChannelRef Channel::createChannel(PipeGroupRef group, LoggerRef logger, const std::string& dir, const std::string& alias, const bool server) {
	return getChannel(group, logger, dir, Util::randomId(), alias, server);
}

ChannelRef Channel::getChannel(PipeGroupRef group, LoggerRef logger, const std::string& dir, const std::string& name, const std::string& alias, const bool server) {
	return impl::ChannelImpl::newChannel(group, logger, dir, name, alias, server);
}

} // namespace Ipc

namespace impl {

Ipc::PipeRef getInputPipe(Ipc::ChannelRef channel)
{
	if(!channel) {
		return Ipc::PipeRef();
	}
	return ((Berkelium::Ipc::impl::ChannelImpl*)channel.get())->getInputPipe();
}

} // namespace impl

} // namespace Berkelium
