// Copyright (c) 2012 The Berkelium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "berkelium/Pipe.hpp"
#include "berkelium/Ipc.hpp"
#include "berkelium/Impl.hpp"

#include <boost/filesystem.hpp>

#include <iostream>

using boost::filesystem::path;

namespace Berkelium {

namespace impl {

Ipc::Ipc() {
}

Ipc::~Ipc() {
}

class IpcImpl : public Ipc {
private:
	const std::string name;
	const bool server;
	PipeRef pin;
	PipeRef pout;

	static inline std::string getExt(const bool server) {
		return server ? "1" : "2";
	}

public:
	IpcImpl(const path& dir, const std::string& name, const bool server) :
		name(name),
		server(server),
		pin(Pipe::getPipe((dir / name).string() + getExt(server))),
		pout(Pipe::getPipe((dir / name).string() + getExt(!server))) {
	}

	virtual ~IpcImpl() {
	}

	// Returns true if there are no pending messages to receive.
	virtual bool isEmpty() {
		return pin->isEmpty();
	}

	// Sends this message.
	virtual void send(const std::string& msg) {
		//std::cerr << "Ipc" << (server ? "Server" : "Client ") << name << " send '" << msg << "'" << std::endl;
		pout->send(msg);
	}

	// Receives the next message.
	virtual const std::string recv() {
		//std::cerr << "Ipc" << (server ? "Server" : "Client ") << name << " recv..." << std::endl;
		std::string ret = pin->recv();
		//std::cerr << "Ipc" << (server ? "Server" : "Client ") << name << " recv '" << ret << "'" << std::endl;
		return ret;
	}

	virtual std::string getName() {
		return name;
	}
};

IpcRef Ipc::getIpc(const std::string& dir, const bool server) {
	return getIpc(dir, randomId(), server);
}
IpcRef Ipc::getIpc(const std::string& dir, const std::string& name, const bool server) {
	return IpcRef(new impl::IpcImpl(path(dir), name, server));
}

} // namespace impl

} // namespace Berkelium
