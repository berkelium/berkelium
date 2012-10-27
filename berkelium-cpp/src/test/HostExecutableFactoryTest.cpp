// Copyright (c) 2012 The Berkelium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "berkelium/HostExecutableFactory.hpp"
#include "gtest/gtest.h"

#include "test.h"

namespace {

class HostExecutableFactoryTest : public ::testing::Test {
};

TEST_F(HostExecutableFactoryTest, forExecutable) {
	ASSERT_NOT_NULL(Berkelium::HostExecutableFactory::forExecutable(""));
}

TEST_F(HostExecutableFactoryTest, forSystemInstalled) {
	ASSERT_NOT_NULL(Berkelium::HostExecutableFactory::forSystemInstalled());
}

} // namespace
