// Copyright (c) 2013 The Berkelium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BERKELIUM_IPC_PIPE_GROUP_HPP_
#define BERKELIUM_IPC_PIPE_GROUP_HPP_
#pragma once

#ifndef BERKELIUM_CPP_IMPL
# error "This file is intended for internal use only!"
#endif

#include <Berkelium/IPC/Ipc.hpp>

namespace Berkelium {

namespace Ipc {

class PipeGroup {
protected:
	PipeGroup();

public:
	virtual ~PipeGroup() = 0;

	virtual void registerPipe(PipeRef pipe) = 0;

	virtual void unregisterPipe(Pipe* pipe) = 0;

	virtual void registerCallback(ChannelRef channel, PipeCallbackRef callback, bool highPrio) = 0;

	virtual void registerCallback(PipeRef pipe, PipeCallbackRef callback, bool highPrio) = 0;

	virtual bool recv(PipeRef pipe, MessageRef msg, int32_t timeout) = 0;

	virtual void update(int32_t timeout) = 0;

	static PipeGroupRef create();
};

} // namespace Ipc

} // namespace Berkelium

#endif // BERKELIUM_IPC_PIPE_GROUP_HPP_
