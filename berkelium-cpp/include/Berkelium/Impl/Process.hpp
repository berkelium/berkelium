// Copyright (c) 2012 The Berkelium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BERKELIUM_IMPL_PROCESS_HPP_
#define BERKELIUM_IMPL_PROCESS_HPP_
#pragma once

#include <Berkelium/API/Logger.hpp>
#include <Berkelium/IPC/Channel.hpp>

#ifndef BERKELIUM_CPP_IMPL
# error "This file is intended for internal use only!"
#endif

#include <vector>

namespace Berkelium {

namespace impl {

class Process;
typedef std::shared_ptr<Process> ProcessRef;

class Process {
private:
	Ipc::ChannelRef ipc;
	Ipc::ChannelRef ipcout;
	Ipc::ChannelRef ipcerr;

protected:
	const LoggerRef logger;

	inline Process(Ipc::ChannelGroupRef group, LoggerRef logger, const std::string& dir) :
		ipc(Ipc::Channel::createChannel(group, logger, dir, true)),
		ipcout(Ipc::Channel::createChannel(group, logger, dir, true)),
		ipcerr(Ipc::Channel::createChannel(group, logger, dir, true)),
		logger(logger) {
	}

public:
	static ProcessRef create(Ipc::ChannelGroupRef group, LoggerRef logger, const std::string& dir);

	virtual ~Process() = 0;

	virtual bool isRunning() = 0;

	Ipc::ChannelRef getIpcChannel() {
		return ipc;
	}

	Ipc::ChannelRef getIpcOut() {
		return ipcout;
	}

	Ipc::ChannelRef getIpcErr() {
		return ipcerr;
	}

	virtual const bool start(const std::vector<std::string>& args) = 0;
};

} // namespace impl

} // namespace Berkelium

#endif // BERKELIUM_IMPL_PROCESS_HPP_
