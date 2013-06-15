// Copyright (c) 2013 The Berkelium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Berkelium/API/Logger.hpp>
#include <Berkelium/API/Util.hpp>
#include <Berkelium/Impl/Filesystem.hpp>

#include "gtest/gtest.h"

#include "test.h"

using Berkelium::impl::Filesystem;

namespace {

class FileSystemTest : public ::testing::Test {
	DEFINE_LOGGER(FileSystemTest);
};

TEST_F(FileSystemTest, split) {
	USE_LOGGER(split);

	std::vector<std::string> test(Filesystem::split(Filesystem::append("a", "b", "c")));
	ASSERT_EQ(3u, test.size());
	ASSERT_STREQ("a", test[0].c_str());
	ASSERT_STREQ("b", test[1].c_str());
	ASSERT_STREQ("c", test[2].c_str());
}

TEST_F(FileSystemTest, tmp) {
	USE_LOGGER(tmp);

	ASSERT_TRUE(Filesystem::exists("/tmp"));
	ASSERT_FALSE(Filesystem::exists("/abcdefghijklmnopqrstuvwxyz"));
}

TEST_F(FileSystemTest, createDeleteDir) {
	USE_LOGGER(createDeleteDir);

	std::string tmp(Filesystem::getTemp());
	logger->info() << tmp << std::endl;

	std::string dir(Filesystem::append(tmp, Berkelium::Util::randomId()));
	ASSERT_FALSE(Filesystem::exists(dir));
	Filesystem::createDirectories(dir);
	ASSERT_TRUE(Filesystem::exists(dir));
	Filesystem::removeDir(dir);
	ASSERT_FALSE(Filesystem::exists(dir));
}

} // namespace
