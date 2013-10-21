// Copyright (c) 2012 The Berkelium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Berkelium/IPC/Link.hpp>
#include <Berkelium/IPC/Message.hpp>
#include <Berkelium/API/Util.hpp>
#include <Berkelium/Impl/Filesystem.hpp>

#include "gtest/gtest.h"
#include "test.h"

using Berkelium::Ipc::Link;
using Berkelium::Ipc::LinkRef;
using Berkelium::Ipc::LinkGroupRef;
using Berkelium::Ipc::Message;
using Berkelium::Ipc::MessageRef;
using Berkelium::Util::randomId;
using Berkelium::impl::Filesystem;

namespace {

class LinkTest : public ::testing::Test {
	DEFINE_LOGGER(LinkTest);
};

TEST_F(LinkTest, create) {
	USE_LOGGER(create);
	ASSERT_NOT_NULL(Link::getLink(false, LinkGroupRef(), logger, Filesystem::getTemp(), randomId(), "testCreateLink"));
}

TEST_F(LinkTest, remove) {
	USE_LOGGER(create);
	std::string dir = Filesystem::append(Filesystem::getTemp(), randomId());
	std::string name(randomId());
	ASSERT_FALSE(Filesystem::exists(dir));
	{
		LinkRef pipe = Link::getLink(false, LinkGroupRef(), getLogger("remove"), dir, name, "testRemoveLink");
		ASSERT_TRUE(Filesystem::exists(dir));
		ASSERT_TRUE(Filesystem::exists(Filesystem::append(dir, name)));
	}
	ASSERT_FALSE(Filesystem::exists(Filesystem::append(dir, name)));
	Filesystem::removeDir(dir);
}

TEST_F(LinkTest, sendRecv) {
	USE_LOGGER(create);
	std::string dir = Filesystem::append(Filesystem::getTemp(), randomId());
		std::string name(randomId());
	ASSERT_FALSE(Filesystem::exists(dir));
	{
		LinkRef pipe = Link::getLink(false, LinkGroupRef(), getLogger("sendRecv"), dir, name, "testSendRecvLink");
		ASSERT_TRUE(Filesystem::exists(dir));
		ASSERT_TRUE(Filesystem::exists(Filesystem::append(dir, name)));
		ASSERT_TRUE(pipe->isEmpty());
		MessageRef msg(Message::create(logger));
		msg->add_str("hello");
		pipe->send(msg);
		ASSERT_FALSE(pipe->isEmpty());
		msg = pipe->recv();
		std::string recv = msg->get_str();
		ASSERT_TRUE(pipe->isEmpty());
		ASSERT_EQ(0, recv.compare("hello"));
	}
	ASSERT_FALSE(Filesystem::exists(Filesystem::append(dir, name)));
	Filesystem::removeDir(dir);
}

} // namespace
