// Copyright (c) 2013 The Berkelium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Berkelium/IPC/ChannelGroup.hpp>
#include <Berkelium/IPC/PipeGroup.hpp>
#include <Berkelium/IPC/Pipe.hpp>
#include <Berkelium/IPC/Message.hpp>
#include <Berkelium/Impl/Impl.hpp>
#include <Berkelium/API/Util.hpp>

#include <map>

namespace Berkelium {

namespace Ipc {

namespace impl {

typedef std::map<int32_t, ChannelRef> ChannelMap;

class ChannelGroupImpl : public ChannelGroup, public PipeCallback {
private:
	ChannelGroupWRef self;
	ChannelGroupImpl* real;
	ChannelGroupImpl* other;
	LoggerRef logger;
	const std::string dir;
	const std::string name;
	PipeGroupRef group;
	ChannelGroupRef reverseRef;
	ChannelGroupWRef reverseWRef;
	int32_t nextId;
	ChannelMap map;
	Berkelium::Ipc::PipeCallbackRef cb;
	PipeRef pin;
	PipeRef pout;

	static inline std::string getExt(const bool server, const bool reverse) {
		if(reverse) {
			return server ? "3" : "4";
		}
		return server ? "1" : "2";
	}

	ChannelGroupImpl(LoggerRef logger, const std::string& dir, const std::string& name, const std::string& alias, PipeGroupRef group, const bool server, const bool reverse) :
		ChannelGroup(),
		self(),
		real(NULL),
		other(NULL),
		logger(logger),
		dir(dir),
		name(name),
		group(group),
		reverseRef(),
		reverseWRef(),
		nextId(0),
		map(),
		cb(),
		pin(Pipe::getPipe(true, group, logger, dir, name + getExt(server, reverse), alias + getExt(server, reverse) + (server ? "Server" : "") + (reverse ? "Reverse" : "") + "In")),
		pout(Pipe::getPipe(false, group, logger, dir, name + getExt(!server, reverse), alias + getExt(!server, reverse) + (!server ? "Server" : "") + (reverse ? "Reverse" : "") + "Out")) {
	}

public:

	virtual ~ChannelGroupImpl() {
	}

	virtual std::string getName() {
		return name;
	}

	virtual ChannelRef getChannel(int32_t id, const std::string& alias) {
		if(real) {
			return real->getChannel(id, alias)->getReverseChannel();
		}
		ChannelRef ret(Berkelium::impl::createChannel(logger, id, self.lock(), alias));
		map.insert(std::pair<int32_t, ChannelRef>(id, ret));
		other->map.insert(std::pair<int32_t, ChannelRef>(id, ret->getReverseChannel()));
		return ret;
	}

	virtual ChannelRef createChannel(const std::string& alias) {
		if(real) {
			// nextId is unused in this instance
			return real->createChannel(alias)->getReverseChannel();
		}
		// nextId is only valid if "real" is not set
		int32_t id;
		if(Berkelium::impl::isBerkeliumHostMode()) {
			// host will create channel ids < 0
			id = --nextId;
		} else {
			// library will create channel ids >= 0
			id = nextId++;
		}
		fprintf(stderr, "allocated channel id %d\n", id);
		return getChannel(id, alias);
	}

	virtual ChannelGroupRef getReverse() {
		if(reverseRef) {
			return reverseRef;
		}
		return reverseWRef.lock();
	}

	virtual void send(int32_t id, Ipc::MessageRef msg) {
		msg->setChannelId(id);
		pout->send(msg);
	}

	virtual void update(int32_t timeout) {
		if(group) {
			group->update(timeout);
		}
	}

	virtual void onPipeDataReady(PipeRef pipe) {
		Ipc::MessageRef msg(pin->recv());
		ChannelMap::iterator it(map.find(msg->getChannelId()));
		if(it == map.end()) {
			logger->error("Received invalid Channel ID");
			return;
		}
		ChannelRef ch(it->second);
		if(!ch) {
			logger->error("Channel already closed!");
			return;
		}
		ch->queue(msg);
	}

	static ChannelGroupRef create(LoggerRef logger, const std::string& dir, const std::string& name, const std::string& alias, PipeGroupRef group, bool server) {
		ChannelGroupImpl* impl1 = new ChannelGroupImpl(logger, dir, name, alias, group, server, false);
		ChannelGroupImpl* impl2 = new ChannelGroupImpl(logger, dir, name, alias, group, server, true);
		ChannelGroupRef ret1(impl1);
		ChannelGroupRef ret2(impl2);

		impl1->self = ret1;
		impl2->self = ret2;
		impl2->real = impl1;
		impl1->other = impl2;
		impl1->reverseRef = ret2;
		impl2->reverseWRef = ret1;

		impl1->cb.reset(new PipeCallbackDelegate<ChannelGroup, ChannelGroupImpl>(ret1));
		impl1->group->registerCallback(impl1->pin, impl1->cb);

		impl2->cb.reset(new PipeCallbackDelegate<ChannelGroup, ChannelGroupImpl>(ret2));
		impl2->group->registerCallback(impl2->pin, impl2->cb);

		return ret1;
	}

	Ipc::PipeRef getInputPipe() {
		return pin;
	}
};

} // namespace impl

ChannelGroup::ChannelGroup() {
}

ChannelGroup::~ChannelGroup() {
}

ChannelGroupRef ChannelGroup::createGroup(LoggerRef logger, const std::string& dir, const std::string& name, const std::string& alias, PipeGroupRef group)
{
	return createGroup(logger, dir, name, alias, group, Berkelium::impl::isBerkeliumHostMode());
}

ChannelGroupRef ChannelGroup::createGroup(LoggerRef logger, const std::string& dir, const std::string& name, const std::string& alias, PipeGroupRef group, bool server)
{
	if(!group) {
		return ChannelGroupRef();
	}
	return impl::ChannelGroupImpl::create(logger, dir, name, alias, group, server);
}

} // namespace Ipc

namespace impl {

Ipc::PipeRef getInputPipe(Ipc::ChannelGroupRef group)
{
	if(!group) {
		return Ipc::PipeRef();
	}
	return ((Berkelium::Ipc::impl::ChannelGroupImpl*)group.get())->getInputPipe();
}

} // namespace impl

} // namespace Berkelium
