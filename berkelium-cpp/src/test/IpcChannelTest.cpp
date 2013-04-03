// Copyright (c) 2012 The Berkelium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Berkelium/IPC/Message.hpp>
#include <Berkelium/IPC/Channel.hpp>
#include <Berkelium/API/Util.hpp>
#include <Berkelium/API/Profile.hpp>
#include <Berkelium/API/BerkeliumFactory.hpp>
#include <Berkelium/Impl/Logger.hpp>

#include "gtest/gtest.h"
#include "test.h"

#include <boost/filesystem.hpp>

using Berkelium::Ipc::Message;
using Berkelium::Ipc::MessageRef;
using Berkelium::Ipc::Channel;
using Berkelium::Ipc::ChannelRef;
using namespace boost::filesystem;

namespace {

class ChannelTest : public ::testing::Test {
};

std::string createTempPath(Berkelium::ProfileRef& profile) {
	profile = Berkelium::BerkeliumFactory::createTemporaryProfile();
	path path = profile->getProfilePath();
	return path.string();
}

TEST_F(ChannelTest, simple) {
	Berkelium::ProfileRef profile;
	std::string name = createTempPath(profile);

	ChannelRef server = Channel::createChannel(name, true);
	ASSERT_NOT_NULL(server);
	ChannelRef client = Channel::getChannel(name, server->getName(), false);
	ASSERT_NOT_NULL(client);

	const int32_t r = 1234;

	MessageRef send(Message::create());
	ASSERT_EQ(0u, send->length());
	ASSERT_EQ(0u, send->remaining());
	send->add_32(r);
	ASSERT_EQ(4u, send->length());
	ASSERT_EQ(4u, send->remaining());

	server->send(send);

	MessageRef recv(Message::create());
	ASSERT_EQ(0u, recv->length());
	ASSERT_EQ(0u, recv->remaining());

	client->recv(recv);
	ASSERT_EQ(4u, recv->length());
	ASSERT_EQ(4u, recv->remaining());
	ASSERT_EQ(r, recv->get_32());
	ASSERT_EQ(4u, recv->length());
	ASSERT_EQ(0u, recv->remaining());
}

TEST_F(ChannelTest, dual) {
	Berkelium::ProfileRef profile;
	std::string name = createTempPath(profile);

	ChannelRef s1 = Channel::createChannel(name, true);
	ASSERT_NOT_NULL(s1);
	ChannelRef s2 = s1->getReverseChannel();
	ASSERT_NOT_NULL(s2);
	ChannelRef c1 = Channel::getChannel(name, s1->getName(), false);
	ASSERT_NOT_NULL(c1);
	ChannelRef c2 = c1->getReverseChannel();
	ASSERT_NOT_NULL(c2);

	MessageRef send(Message::create());

	send->add_32(0x1234567);
	s1->send(send);

	send->add_32(0x2345678);
	s1->send(send);

	send->add_32(0x3456789);
	s2->send(send);

	send->add_32(0x4567890);
	s2->send(send);

	c1->recv(send);
	ASSERT_EQ(0x1234567, send->get_32());

	c2->recv(send);
	ASSERT_EQ(0x3456789, send->get_32());

	c2->recv(send);
	ASSERT_EQ(0x4567890, send->get_32());

	c1->recv(send);
	ASSERT_EQ(0x2345678, send->get_32());
}

TEST_F(ChannelTest, free) {
	Berkelium::ProfileRef profile;
	std::string name = createTempPath(profile);

	ChannelRef c1 = Channel::createChannel(name, true);
	ASSERT_TRUE(c1.unique());
	void* org = c1.get();
	ChannelRef c2 = c1;
	ASSERT_FALSE(c2.unique());
	ASSERT_EQ(org, c2.get());

	c1.reset();
	ASSERT_NOT_NULL(c2);
	ASSERT_TRUE(c2.unique());

	c1 = c2->getReverseChannel();
	ASSERT_EQ(2, c1.use_count());
	ASSERT_TRUE(c2.unique());
	ASSERT_NE(c1.get(), c2.get());

	c2.reset();
	// new reverse channel object should not be placed on the same location
	// where c2 was stored before, so a dummy channel is created here
	ChannelRef dummy = Channel::createChannel(name, true);

	c2 = c1->getReverseChannel();
	ASSERT_NULL(c2);
}

} // namespace
