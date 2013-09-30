// Copyright (c) 2012 The Berkelium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BERKELIUM_IPC_CHANNEL_HPP_
#define BERKELIUM_IPC_CHANNEL_HPP_
#pragma once

#ifndef BERKELIUM_CPP_IMPL
# error "This file is intended for internal use only!"
#endif

#include <Berkelium/IPC/Ipc.hpp>

namespace Berkelium {

namespace Ipc {

class ChannelCallback
{
public:
	virtual ~ChannelCallback() = 0;

	virtual void onDataReady(ChannelRef channel) = 0;
};

class Channel {
protected:
	Channel();

public:
	static ChannelRef createChannel(ChannelGroupRef group, LoggerRef logger, const std::string& dir, const bool server);
	static ChannelRef getChannel(ChannelGroupRef group, LoggerRef logger, const std::string& dir, const std::string& name, const bool server);

	virtual ~Channel() = 0;

	// Returns true if there are no pending messages to receive.
	virtual bool isEmpty() = 0;

	// Returns the Channel default Message.
	virtual MessageRef getMessage() = 0;

	// Sends this message.
	virtual void send(MessageRef msg) = 0;

	// Receives the next message.
	virtual void recv(MessageRef msg) = 0;

 	virtual ChannelRef createSubChannel() = 0;
 	virtual ChannelRef getSubChannel(const std::string& name) = 0;

	virtual ChannelRef getReverseChannel() = 0;

	virtual std::string getName() = 0;

	virtual ChannelGroupRef getGroup() = 0;
};

} // namespace Ipc

} // namespace Berkelium

#endif // BERKELIUM_IPC_CHANNEL_HPP_
