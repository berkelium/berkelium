// Copyright (c) 2013 The Berkelium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Berkelium/API/Util.hpp>
#include <Berkelium/Impl/Impl.hpp>
#include <Berkelium/Impl/Filesystem.hpp>
#include <Berkelium/IPC/Link.hpp>
#include <Berkelium/IPC/ChannelGroup.hpp>

namespace Berkelium {

namespace impl {

Process::Process(RuntimeRef runtime, LoggerRef logger, const std::string& dir) :
	name(Berkelium::Util::randomId()),
	group(getLinkGroup(runtime)),
	channels(Ipc::ChannelGroup::createGroup(logger, dir, Util::randomId(), "process", group, true)),
	ipc(channels ? channels->createChannel("process.ipc") : NULL),
	out(runtime, LogType::StdOut),
	err(runtime, LogType::StdErr),
	bufout(),
	buferr(),
	logger(logger) {
	TRACE_OBJECT_NEW("Process");
}

Process::~Process() {
	TRACE_OBJECT_DELETE("Process");
}

} // namespace impl

} // namespace Berkelium
