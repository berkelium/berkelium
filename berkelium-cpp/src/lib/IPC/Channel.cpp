// Copyright (c) 2012 The Berkelium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Berkelium/API/Util.hpp>
#include <Berkelium/API/Logger.hpp>
#include <Berkelium/IPC/Pipe.hpp>
#include <Berkelium/IPC/Message.hpp>
#include <Berkelium/IPC/PipeGroup.hpp>
#include <Berkelium/IPC/ChannelGroup.hpp>
#include <Berkelium/Impl/Impl.hpp>

namespace Berkelium {

namespace Ipc {

namespace impl {

class ChannelImpl : public Channel {
private:
	ChannelGroupRef group;
	LoggerRef logger;
	const std::string alias;
	ChannelRef reverseRef;
	ChannelWRef reverseWRef;
	ChannelWRef self;
	ChannelCallbackRef cb;
	std::list<MessageRef> messages;
	bool reading;
	int32_t id;

public:
	ChannelImpl(LoggerRef logger, int32_t id, ChannelGroupRef group, const std::string& alias) :
		Channel(),
		group(group),
		logger(logger),
		alias(alias),
		reverseRef(),
		reverseWRef(),
		self(),
		cb(),
		messages(),
		reading(false),
		id(id) {
	}

	virtual ~ChannelImpl() {
	}

	virtual void registerCallback(ChannelCallbackRef cb) {
		this->cb = cb;
	}

	virtual void send(Ipc::MessageRef msg) {
		group->send(id, msg);
	}

	virtual MessageRef recv() {
		return recv(-1);
	}

	virtual MessageRef recv(int32_t timeout) {
		reading = true;
		int64_t end = Util::currentTimeMillis() + timeout;
		while(messages.empty()) {
			if(timeout == -1) {
				group->update(-1);
			} else {
				int64_t now = Util::currentTimeMillis();
				int64_t left = end - now;
				if(left < 1) {
					reading = false;
					return MessageRef();
				}
				group->update((int32_t) left);
			}
		}
		MessageRef ret(messages.front());
		messages.pop_front();
		reading = false;
		return ret;
	}

	virtual void queue(MessageRef msg) {
		if(reading || !cb) {
			messages.push_back(msg);
		} else {
			cb->onDataReady(self.lock(), msg);
		}
	}

	virtual ChannelRef getReverseChannel() {
		if(reverseRef) {
			return reverseRef;
		}
		return reverseWRef.lock();
	}

	virtual bool isReverseChannel() {
		return !reverseRef;
	}

	virtual int32_t getId() {
		return id;
	}

	virtual std::string getAlias() {
		return alias;
	}

	virtual ChannelGroupRef getGroup() {
		return group;
	}

	static ChannelRef create(LoggerRef logger, int32_t id, ChannelGroupRef group, const std::string& alias) {
		ChannelImpl* impl1 = new ChannelImpl(logger, id, group, alias);
		ChannelImpl* impl2 = new ChannelImpl(logger, id, group->getReverse(), alias);
		ChannelRef ret1(impl1);
		ChannelRef ret2(impl2);
		impl1->self = ret1;
		impl2->self = ret2;
		impl1->reverseRef = ret2;
		impl2->reverseWRef = ret1;
		return ret1;
	}
};

} // namespace impl

Channel::Channel() {
}

Channel::~Channel() {
}

ChannelCallback::ChannelCallback() {
}

ChannelCallback::~ChannelCallback() {
}

} // namespace Ipc

namespace impl {

Ipc::ChannelRef createChannel(LoggerRef logger, int32_t id, Ipc::ChannelGroupRef group, const std::string& alias)
{
	if(!group) {
		return Ipc::ChannelRef();
	}
	return Berkelium::Ipc::impl::ChannelImpl::create(logger, id, group, alias);
}

} // namespace impl

} // namespace Berkelium
