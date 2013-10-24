// Copyright (c) 2012 The Berkelium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Berkelium/IPC/Message.hpp>
#include <Berkelium/IPC/Channel.hpp>
#include <Berkelium/IPC/LinkGroup.hpp>
#include <Berkelium/IPC/ChannelGroup.hpp>
#include <Berkelium/API/Runtime.hpp>
#include <Berkelium/API/Util.hpp>
#include <Berkelium/API/Profile.hpp>
#include <Berkelium/API/BerkeliumFactory.hpp>
#include <Berkelium/Impl/Impl.hpp>

#include "gtest/gtest.h"
#include "test.h"

using Berkelium::Ipc::Message;
using Berkelium::Ipc::MessageRef;
using Berkelium::Ipc::Channel;
using Berkelium::Ipc::ChannelRef;
using Berkelium::Ipc::ChannelGroup;
using Berkelium::Ipc::ChannelGroupRef;
using Berkelium::Ipc::LinkGroup;
using Berkelium::Ipc::LinkGroupRef;

namespace {

class ChannelTest : public ::testing::Test {
	DEFINE_LOGGER(ChannelTest);
};

std::string createTempPath(Berkelium::ProfileRef& profile) {
	return Berkelium::BerkeliumFactory::getDefaultRuntime()->createTemporaryProfile()->getProfilePath();
}

TEST_F(ChannelTest, objectCount) {
	USE_LOGGER(objectCount);
	std::string initial(Berkelium::impl::getBerkeliumObjectCount());
	Berkelium::ProfileRef profile;
	std::string dir = createTempPath(profile);
	std::string name(Berkelium::Util::randomId());
	LinkGroupRef pipeGroup(LinkGroup::create());
	ChannelGroupRef channels(ChannelGroup::createGroup(logger, dir, name, "test client", pipeGroup, false));
	ASSERT_STRNE(initial.c_str(), Berkelium::impl::getBerkeliumObjectCount().c_str());
	pipeGroup.reset();
	ASSERT_STRNE(initial.c_str(), Berkelium::impl::getBerkeliumObjectCount().c_str());
	profile.reset();
	ASSERT_STRNE(initial.c_str(), Berkelium::impl::getBerkeliumObjectCount().c_str());
	ChannelRef channel(channels->createChannel("process.ipc"));
	ASSERT_STRNE(initial.c_str(), Berkelium::impl::getBerkeliumObjectCount().c_str());
	channels.reset();
	ASSERT_STRNE(initial.c_str(), Berkelium::impl::getBerkeliumObjectCount().c_str());
	channel.reset();
	ASSERT_STREQ(initial.c_str(), Berkelium::impl::getBerkeliumObjectCount().c_str());
}

TEST_F(ChannelTest, simple) {
	USE_LOGGER(simple);
	Berkelium::ProfileRef profile;
	std::string dir = createTempPath(profile);
	std::string name(Berkelium::Util::randomId());
	LinkGroupRef pipeGroup(LinkGroup::create());
	ChannelGroupRef clientGroup(ChannelGroup::createGroup(logger, dir, name, "test client", pipeGroup, false));
	ChannelGroupRef serverGroup(ChannelGroup::createGroup(logger, dir, name, "test server", pipeGroup, true));

	for(int32_t i = 0; i < 1024; i++) {
		ChannelRef server = clientGroup->createChannel("test");
		ASSERT_NOT_NULL(server);
		ChannelRef client = serverGroup->getChannel(server->getId(), "test");
		ASSERT_NOT_NULL(client);

		const int32_t r = 1234 * i;
		//fprintf(stderr, "i:%d r:%d\n", i, r);

		MessageRef send(Message::create(logger));
		ASSERT_EQ(0u, send->length());
		ASSERT_EQ(0u, send->remaining());
		send->add_32(r);
		ASSERT_EQ(4u, send->length());
		ASSERT_EQ(4u, send->remaining());

		server->send(send);

		MessageRef recv = client->recv();
		ASSERT_EQ(4u, recv->length());
		ASSERT_EQ(4u, recv->remaining());
		ASSERT_EQ(r, recv->get_32());
		ASSERT_EQ(4u, recv->length());
		ASSERT_EQ(0u, recv->remaining());
	}
}

TEST_F(ChannelTest, dual) {
	USE_LOGGER(dual);
	Berkelium::ProfileRef profile;
	std::string dir = createTempPath(profile);
	std::string name(Berkelium::Util::randomId());
	LinkGroupRef pipeGroup(LinkGroup::create());
	ChannelGroupRef clientGroup(ChannelGroup::createGroup(logger, dir, name, "test clientGroup", pipeGroup, false));
	ChannelGroupRef serverGroup(ChannelGroup::createGroup(logger, dir, name, "test server", pipeGroup, true));

	ChannelRef s1 = clientGroup->createChannel("test");
	ASSERT_NOT_NULL(s1);
	ChannelRef s2 = s1->getReverseChannel();
	ASSERT_NOT_NULL(s2);
	ChannelRef c1 = serverGroup->getChannel(s1->getId(), "test");
	ASSERT_NOT_NULL(c1);
	ChannelRef c2 = c1->getReverseChannel();
	ASSERT_NOT_NULL(c2);

	MessageRef send(Message::create(logger));

	send->add_32(0x1234567);
	s1->send(send);

	send->add_32(0x2345678);
	s1->send(send);

	send->add_32(0x3456789);
	s2->send(send);

	send->add_32(0x4567890);
	s2->send(send);

	send = c1->recv();
	ASSERT_EQ(0x1234567, send->get_32());

	send = c2->recv();
	ASSERT_EQ(0x3456789, send->get_32());

	send = c2->recv();
	ASSERT_EQ(0x4567890, send->get_32());

	send = c1->recv();
	ASSERT_EQ(0x2345678, send->get_32());
}

TEST_F(ChannelTest, free) {
	USE_LOGGER(free);
	Berkelium::ProfileRef profile;
	std::string dir = createTempPath(profile);
	LinkGroupRef pipeGroup(LinkGroup::create());
	std::string name(Berkelium::Util::randomId());
	ChannelGroupRef group(ChannelGroup::createGroup(logger, dir, name, "test clientGroup", pipeGroup, false));

	ChannelRef c1 = group->createChannel("test");
	ASSERT_EQ(2, c1.use_count());
	void* org = c1.get();
	ChannelRef c2 = c1;
	ASSERT_EQ(3, c2.use_count());
	ASSERT_EQ(org, c2.get());

	c1.reset();
	ASSERT_NOT_NULL(c2);
	ASSERT_EQ(2, c2.use_count());

	c1 = c2->getReverseChannel();
	ASSERT_EQ(3, c1.use_count());
	ASSERT_EQ(2, c2.use_count());
	ASSERT_NE(c1.get(), c2.get());

	c2.reset();
	// new reverse channel object should not be placed on the same location
	// where c2 was stored before, so a dummy channel is created here
	ChannelRef dummy = group->getChannel(c1->getId(), "test");

	c2 = c1->getReverseChannel();
	ASSERT_EQ(2, c2.use_count());
}

} // namespace
