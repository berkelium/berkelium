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

class Process {
protected:
	std::string name;
	Ipc::PipeGroupRef group;
	Ipc::ChannelGroupRef channels;
	Ipc::ChannelRef ipc;
	Ipc::PipeRef pipeout;
	Ipc::PipeRef pipeerr;

	const LoggerRef logger;

	Process(RuntimeRef runtime, LoggerRef logger, const std::string& dir);

public:
	static ProcessRef create(RuntimeRef runtime, LoggerRef logger, const std::string& dir);

	virtual ~Process() = 0;

	virtual bool isRunning() = 0;

	inline Ipc::ChannelGroupRef getChannelGroup() {
		return channels;
	}

	inline Ipc::ChannelRef getIpcChannel() {
		return ipc;
	}

	virtual const bool start(const std::vector<std::string>& args) = 0;
};

} // namespace impl

} // namespace Berkelium

#endif // BERKELIUM_IMPL_PROCESS_HPP_
