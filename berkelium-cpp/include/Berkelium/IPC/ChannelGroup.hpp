// Copyright (c) 2013 The Berkelium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BERKELIUM_IPC_CHANNEL_GROUP_HPP_
#define BERKELIUM_IPC_CHANNEL_GROUP_HPP_
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

	virtual std::string getName() = 0;

	virtual void waitForInit() = 0;

	virtual void send(int32_t id, Ipc::MessageRef msg) = 0;

	virtual void update(int32_t timeout) = 0;

	virtual ChannelRef getChannel(int32_t id, const std::string& alias) = 0;

	virtual ChannelRef createChannel(const std::string& alias) = 0;

	static ChannelGroupRef createGroup(LoggerRef logger, const std::string& dir, const std::string& name, const std::string& alias, LinkGroupRef group, bool server);
};

} // namespace Ipc

} // namespace Berkelium

#endif // BERKELIUM_IPC_CHANNEL_GROUP_HPP_
