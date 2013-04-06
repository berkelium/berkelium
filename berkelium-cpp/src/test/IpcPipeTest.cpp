// Copyright (c) 2012 The Berkelium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Berkelium/IPC/Pipe.hpp>
#include <Berkelium/IPC/Message.hpp>
#include <Berkelium/API/Util.hpp>

#include "gtest/gtest.h"
#include "test.h"

#include <boost/filesystem.hpp>

using Berkelium::Ipc::Pipe;
using Berkelium::Ipc::PipeRef;
using Berkelium::Ipc::Message;
using Berkelium::Ipc::MessageRef;
using Berkelium::Util::randomId;

namespace {

class PipeTest : public ::testing::Test {
	DEFINE_LOGGER(PipeTest);
};

TEST_F(PipeTest, create) {
	USE_LOGGER(create);
	ASSERT_NOT_NULL(Pipe::getPipe(logger, randomId()));
}

TEST_F(PipeTest, remove) {
	USE_LOGGER(create);
	std::string name = "/tmp/berkelium." + randomId();
	ASSERT_FALSE(boost::filesystem::exists(name));
	{
		PipeRef pipe = Pipe::getPipe(getLogger("remove"), name);
		ASSERT_TRUE(boost::filesystem::exists(name));
	}
	ASSERT_FALSE(boost::filesystem::exists(name));
}

TEST_F(PipeTest, sendRecv) {
	USE_LOGGER(create);
	std::string name = "/tmp/berkelium." + randomId();
	ASSERT_FALSE(boost::filesystem::exists(name));
	{
		PipeRef pipe = Pipe::getPipe(getLogger("sendRecv"), name);
		ASSERT_TRUE(boost::filesystem::exists(name));
		ASSERT_TRUE(pipe->isEmpty());
		MessageRef msg(Message::create(logger));
		msg->add_str("hello");
		pipe->send(msg);
		ASSERT_FALSE(pipe->isEmpty());
		msg = Message::create(logger);
		pipe->recv(msg);
		std::string recv = msg->get_str();
		ASSERT_TRUE(pipe->isEmpty());
		ASSERT_EQ(0, recv.compare("hello"));
	}
	ASSERT_FALSE(boost::filesystem::exists(name));
}

} // namespace
