// Copyright (c) 2012 The Berkelium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Berkelium/API/Util.hpp>
#include <Berkelium/IPC/Pipe.hpp>
#include <Berkelium/IPC/Ipc.hpp>
#include <Berkelium/Impl/Impl.hpp>

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
	PipeRef pin;
	PipeRef pout;

	static inline std::string getExt(const bool server) {
		return server ? "1" : "2";
	}

public:
	ChannelImpl(const path& dir, const std::string& name, const bool server) :
		dir(dir.string()),
		name(name),
		server(server),
		pin(Pipe::getPipe((dir / name).string() + getExt(server))),
		pout(Pipe::getPipe((dir / name).string() + getExt(!server))) {
	}

	virtual ~ChannelImpl() {
	}

	// Returns true if there are no pending messages to receive.
	virtual bool isEmpty() {
		return pin->isEmpty();
	}

	// Sends this message.
	virtual void send(Ipc::MessageRef msg) {
		pout->send(msg);
	}

	// Receives the next message.
	virtual void recv(Ipc::MessageRef msg) {
		pin->recv(msg);
	}

	virtual ChannelRef createSubChannel() {
		return Channel::getChannel(dir, true);
	}

	virtual ChannelRef getSubChannel(const std::string& name) {
		return Channel::getChannel(dir, name, false);
	}

	virtual std::string getName() {
		return name;
	}
};

} // namespace impl

Channel::Channel() {
}

Channel::~Channel() {
}

ChannelRef Channel::getChannel(const std::string& dir, const bool server) {
	return getChannel(dir, Util::randomId(), server);
}
ChannelRef Channel::getChannel(const std::string& dir, const std::string& name, const bool server) {
	return ChannelRef(new impl::ChannelImpl(path(dir), name, server));
}

} // namespace IPC

} // namespace Berkelium
