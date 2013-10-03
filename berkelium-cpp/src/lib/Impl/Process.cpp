// Copyright (c) 2013 The Berkelium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Berkelium/API/Util.hpp>
#include <Berkelium/Impl/Impl.hpp>
#include <Berkelium/Impl/Filesystem.hpp>
#include <Berkelium/IPC/Pipe.hpp>

namespace Berkelium {

namespace impl {

Process::Process(RuntimeRef runtime, LoggerRef logger, const std::string& dir) :
	name(Berkelium::Util::randomId()),
	group(getPipeGroup(runtime)),
	ipc(Ipc::Channel::createChannel(group, logger, dir, "process.ipc", true)),
	pipeout(Ipc::Pipe::getPipe(true, group, logger, dir, name + "1", "process.out")),
	pipeerr(Ipc::Pipe::getPipe(true, group, logger, dir, name + "2", "process.err")),
	logger(logger) {
}

Process::~Process()
{
}

} // namespace impl

} // namespace Berkelium
