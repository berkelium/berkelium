// Copyright (c) 2013 The Berkelium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Berkelium/IPC/ChannelGroup.hpp>
#include <Berkelium/IPC/LinkGroup.hpp>
#include <Berkelium/IPC/Link.hpp>
#include <Berkelium/IPC/Message.hpp>
#include <Berkelium/Impl/Impl.hpp>
#include <Berkelium/API/Util.hpp>

#include <map>

namespace Berkelium {

namespace Ipc {

namespace impl {

typedef std::map<int32_t, ChannelWRef> ChannelMap;
typedef std::pair<int32_t, ChannelWRef> ChannelMapPair;

class ChannelGroupImpl : public ChannelGroup, public LinkCallback {
private:
	ChannelGroupWRef self;
	LoggerRef logger;
	const std::string dir;
	const std::string name;
	LinkGroupRef group;
	int32_t nextId;
	ChannelMap map;
	Berkelium::Ipc::LinkCallbackRef cb;
	LinkRef link;

	ChannelGroupImpl(LoggerRef logger, const std::string& dir, const std::string& name, const std::string& alias, LinkGroupRef group, const bool server) :
		ChannelGroup(),
		self(),
		logger(logger),
		dir(dir),
		name(name),
		group(group),
		nextId(0),
		map(),
		cb(),
		link(Link::getLink(server, group, logger, dir, name, alias)) {
		TRACE_OBJECT_NEW("ChannelGroupImpl");
	}

public:

	virtual ~ChannelGroupImpl() {
		TRACE_OBJECT_DELETE("ChannelGroupImpl");
	}

	virtual std::string getName() {
		return name;
	}

	virtual ChannelRef getChannel(int32_t id, const std::string& alias) {
		ChannelRef ret(Berkelium::impl::createChannel(logger, id, self.lock(), alias));
		map.insert(ChannelMapPair(id, ret));
		map.insert(ChannelMapPair(id + 1, ret->getReverseChannel()));
		return ret;
	}

	virtual ChannelRef createChannel(const std::string& alias) {
		// nextId is only valid if "real" is not set
		int32_t id;
		if(Berkelium::impl::isBerkeliumHostMode()) {
			// host will create channel ids < 0
			id = --nextId;
			id *= 2;
			// -2, -4, -6, -8, -10... are default channels,
			// -1, -3, -5, -7, -9... are reverse channels.
		} else {
			// library will create channel ids >= 0
			id = nextId++;
			id *= 2;
			// 0, 2, 4, 6, 8... are default channels,
			// 1, 3, 5, 7, 9... are reverse channels.
		}
		// reverse channel id: see ChannelImpl::create
		// -> id + 1
		fprintf(stderr, "allocated channel id %d\n", id);
		return getChannel(id, alias);
	}

	virtual void send(int32_t id, Ipc::MessageRef msg) {
		msg->setChannelId(id);
		link->send(msg);
	}

	virtual void update(int32_t timeout) {
		if(group) {
			group->update(timeout);
		}
	}

	virtual void onLinkDataReady(LinkRef pipe) {
		Ipc::MessageRef msg(link->recv());
		int32_t id(msg->getChannelId());
		ChannelMap::iterator it(map.find(id));
		if(it == map.end()) {
			logger->error() << "Received invalid Channel ID " << id << std::endl;
			return;
		}
		ChannelRef ch(it->second);
		if(!ch) {
			logger->error("Channel already closed!");
			return;
		}
		ch->queue(msg);
	}

	static ChannelGroupRef create(LoggerRef logger, const std::string& dir, const std::string& name, const std::string& alias, LinkGroupRef group, bool server) {
		ChannelGroupImpl* impl = new ChannelGroupImpl(logger, dir, name, alias, group, server);
		ChannelGroupRef ret(impl);

		impl->self = ret;

		impl->cb.reset(new LinkCallbackDelegate<ChannelGroup, ChannelGroupImpl>(ret));
		impl->group->registerCallback(impl->link, impl->cb);

		return ret;
	}

	Ipc::LinkRef getInputLink() {
		return link;
	}
};

} // namespace impl

ChannelGroup::ChannelGroup() {
	TRACE_OBJECT_NEW("ChannelGroup");
}

ChannelGroup::~ChannelGroup() {
	TRACE_OBJECT_DELETE("ChannelGroup");
}

ChannelGroupRef ChannelGroup::createGroup(LoggerRef logger, const std::string& dir, const std::string& name, const std::string& alias, LinkGroupRef group)
{
	return createGroup(logger, dir, name, alias, group, Berkelium::impl::isBerkeliumHostMode());
}

ChannelGroupRef ChannelGroup::createGroup(LoggerRef logger, const std::string& dir, const std::string& name, const std::string& alias, LinkGroupRef group, bool server)
{
	if(!group) {
		return ChannelGroupRef();
	}
	return impl::ChannelGroupImpl::create(logger, dir, name, alias, group, server);
}

} // namespace Ipc

namespace impl {

Ipc::LinkRef getInputLink(Ipc::ChannelGroupRef group)
{
	if(!group) {
		return Ipc::LinkRef();
	}
	return ((Berkelium::Ipc::impl::ChannelGroupImpl*)group.get())->getInputLink();
}

} // namespace impl

} // namespace Berkelium
