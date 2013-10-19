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
	ChannelCallback();

	virtual ~ChannelCallback() = 0;

	virtual void onChannelDataReady(ChannelRef channel, MessageRef msg) = 0;
};

template<class T, class I>
class ChannelCallbackDelegate : public ChannelCallback {
private:
	std::weak_ptr<T> wref;

public:
	ChannelCallbackDelegate(std::weak_ptr<T> wref) :
		ChannelCallback(),
		wref(wref) {
	}

	virtual ~ChannelCallbackDelegate() {
	}

	virtual void onChannelDataReady(ChannelRef channel, MessageRef msg) {
		std::shared_ptr<T> ref(wref.lock());
		if(ref) {
			I* impl = (I*)ref.get();
			impl->onChannelDataReady(channel, msg);
		}
	}
};

class Channel {
protected:
	Channel();

public:
	virtual ~Channel() = 0;

	// Sends this message.
	virtual void send(MessageRef msg) = 0;

	// Receives the next message. Null if there is no Message.
	virtual MessageRef recv() = 0;

	virtual MessageRef recv(int32_t timeout) = 0;

	virtual void queue(MessageRef msg) = 0;

	virtual ChannelRef getReverseChannel() = 0;

	virtual void registerCallback(ChannelCallbackRef cb) = 0;

	virtual bool isReverseChannel() = 0;

	virtual int32_t getId() = 0;

	virtual std::string getAlias() = 0;

	virtual ChannelGroupRef getGroup() = 0;
};

} // namespace Ipc

} // namespace Berkelium

#endif // BERKELIUM_IPC_CHANNEL_HPP_
