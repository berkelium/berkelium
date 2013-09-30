// Copyright (c) 2013 The Berkelium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BERKELIUM_IPC_CHANNEL_GROUP_HPP_
#define BERKELIUM_IPC_CHANNEL_GROUPHPP_
#pragma once

#ifndef BERKELIUM_CPP_IMPL
# error "This file is intended for internal use only!"
#endif

#include <Berkelium/IPC/Ipc.hpp>

namespace Berkelium {

namespace Ipc {

class ChannelGroup {
protected:
	ChannelGroup();

public:
	virtual ~ChannelGroup() = 0;

	virtual void registerChannel(ChannelRef channel) = 0;

	virtual void unregisterChannel(Channel* channel) = 0;

	virtual void registerCallback(ChannelRef channel, ChannelCallbackRef callback, bool highPrio) = 0;

	virtual void recv(ChannelRef channel, PipeRef pipe, MessageRef msg) = 0;

	virtual void update(int32_t timeout) = 0;

	static ChannelGroupRef create();
};

} // namespace Ipc

} // namespace Berkelium

#endif // BERKELIUM_IPC_CHANNEL_GROUP_HPP_
