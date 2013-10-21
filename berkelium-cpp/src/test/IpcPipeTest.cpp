// Copyright (c) 2012 The Berkelium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Berkelium/IPC/Link.hpp>
#include <Berkelium/IPC/Message.hpp>
#include <Berkelium/API/Util.hpp>
#include <Berkelium/Impl/Filesystem.hpp>

#include "gtest/gtest.h"
#include "test.h"

using Berkelium::Ipc::Pipe;
using Berkelium::Ipc::PipeRef;
using Berkelium::Ipc::PipeGroupRef;
using Berkelium::Ipc::Message;
using Berkelium::Ipc::MessageRef;
using Berkelium::Util::randomId;
using Berkelium::impl::Filesystem;

namespace {

class PipeTest : public ::testing::Test {
	DEFINE_LOGGER(PipeTest);
};

TEST_F(PipeTest, create) {
	USE_LOGGER(create);
	ASSERT_NOT_NULL(Pipe::getPipe(false, PipeGroupRef(), logger, Filesystem::getTemp(), randomId(), "testCreatePipe"));
}

TEST_F(PipeTest, remove) {
	USE_LOGGER(create);
	std::string dir = Filesystem::append(Filesystem::getTemp(), randomId());
	std::string name(randomId());
	ASSERT_FALSE(Filesystem::exists(dir));
	{
		PipeRef pipe = Pipe::getPipe(false, PipeGroupRef(), getLogger("remove"), dir, name, "testRemovePipe");
		ASSERT_TRUE(Filesystem::exists(dir));
		ASSERT_TRUE(Filesystem::exists(Filesystem::append(dir, name)));
	}
	ASSERT_FALSE(Filesystem::exists(Filesystem::append(dir, name)));
	Filesystem::removeDir(dir);
}

TEST_F(PipeTest, sendRecv) {
	USE_LOGGER(create);
	std::string dir = Filesystem::append(Filesystem::getTemp(), randomId());
		std::string name(randomId());
	ASSERT_FALSE(Filesystem::exists(dir));
	{
		PipeRef pipe = Pipe::getPipe(false, PipeGroupRef(), getLogger("sendRecv"), dir, name, "testSendRecvPipe");
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
