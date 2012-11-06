// Copyright (c) 2012 The Berkelium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "berkelium/Pipe.hpp"
#include "berkelium/Impl.hpp"

#include "gtest/gtest.h"
#include "test.h"

#include <boost/filesystem.hpp>

using Berkelium::impl::Pipe;
using Berkelium::impl::PipeRef;

namespace {

class PipeTest : public ::testing::Test {
};

TEST_F(PipeTest, create) {
	ASSERT_NOT_NULL(Pipe::getPipe(Berkelium::impl::randomId()));
}

TEST_F(PipeTest, remove) {
	std::string name = Berkelium::impl::randomId();
	ASSERT_FALSE(boost::filesystem::exists("/tmp/berkelium." + name));
	{
		PipeRef pipe = Pipe::getPipe(name);
		ASSERT_TRUE(boost::filesystem::exists("/tmp/berkelium." + name));
	}
	ASSERT_FALSE(boost::filesystem::exists("/tmp/berkelium." + name));
}

TEST_F(PipeTest, sendRecv) {
	std::string name = Berkelium::impl::randomId();
	ASSERT_FALSE(boost::filesystem::exists("/tmp/berkelium." + name));
	{
		PipeRef pipe = Pipe::getPipe(name);
		ASSERT_TRUE(boost::filesystem::exists("/tmp/berkelium." + name));
		ASSERT_TRUE(pipe->isEmpty());
		pipe->send("hello");
		ASSERT_FALSE(pipe->isEmpty());
		std::string recv = pipe->recv();
		ASSERT_TRUE(pipe->isEmpty());
		ASSERT_EQ(0, recv.compare("hello"));
	}
	ASSERT_FALSE(boost::filesystem::exists("/tmp/berkelium." + name));
}

} // namespace
