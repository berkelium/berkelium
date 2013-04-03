// Copyright (c) 2012 The Berkelium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Berkelium/API/Util.hpp>
#include <Berkelium/IPC/Pipe.hpp>
#include <Berkelium/IPC/Message.hpp>
#include <Berkelium/Impl/Impl.hpp>
#include <Berkelium/Impl/Logger.hpp>

#include <boost/filesystem.hpp>

#include <iostream>

using boost::filesystem::path;

namespace Berkelium {

namespace Ipc {

namespace impl {

class ChannelImpl : public Channel {
private:
	const std::string dir;
	const std::string name;
	const bool server;
	const bool reverse;
	ChannelWRef reverseRef;
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
	ChannelImpl(const path& dir, const std::string& name, const bool server, const bool reverse) :
		dir(dir.string()),
		name(name),
		server(server),
		reverse(reverse),
		reverseRef(),
		self(),
		message(Message::create()),
		pin(Pipe::getPipe((dir / name).string() + getExt(server, reverse))),
		pout(Pipe::getPipe((dir / name).string() + getExt(!server, reverse))) {
	}

	virtual ~ChannelImpl() {
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
		return Channel::createChannel(dir, true);
	}

	virtual ChannelRef getSubChannel(const std::string& name) {
		return Channel::getChannel(dir, name, false);
	}

	virtual ChannelRef getReverseChannel() {
		ChannelRef ret = reverseRef.lock();
		if(ret) {
			return ret;
		}
		impl::ChannelImpl* impl = new impl::ChannelImpl(path(dir), name, server, !reverse);

		ret = ChannelRef(impl);
		reverseRef = ret;
		return ret;
	}

	virtual std::string getName() {
		return name;
	}

	ChannelRef getSelf() {
		return self.lock();
	}

	static ChannelRef newChannel(const std::string& dir, const std::string& name, const bool server) {
		ChannelImpl* impl = new ChannelImpl(path(dir), name, server, false);
		ChannelRef ret(impl);
		impl->self = ret;
		return ret;
	}
};

} // namespace impl

Channel::Channel() {
}

Channel::~Channel() {
}

ChannelRef Channel::createChannel(const std::string& dir, const bool server) {
	return getChannel(dir, Util::randomId(), server);
}

ChannelRef Channel::getChannel(const std::string& dir, const std::string& name, const bool server) {
	return impl::ChannelImpl::newChannel(dir, name, server);
}

} // namespace Ipc

} // namespace Berkelium
