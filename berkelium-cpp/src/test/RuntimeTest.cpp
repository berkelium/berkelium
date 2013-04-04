// Copyright (c) 2012 The Berkelium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Berkelium/API/Runtime.hpp>
#include <Berkelium/API/BerkeliumFactory.hpp>
#include <Berkelium/API/HostExecutable.hpp>

#include "gtest/gtest.h"

#include "test.h"

namespace {

class RuntimeTest : public ::testing::Test {
};

TEST_F(RuntimeTest, forExecutable) {
	ASSERT_NOT_NULL(Berkelium::BerkeliumFactory::getDefaultRuntime()->forExecutable(""));
}

TEST_F(RuntimeTest, forSystemInstalled) {
	ASSERT_NOT_NULL(Berkelium::BerkeliumFactory::getDefaultRuntime()->forSystemInstalled());
}

TEST_F(RuntimeTest, forProfile) {
	ASSERT_NOT_NULL(Berkelium::BerkeliumFactory::getDefaultRuntime()->forProfile(""));
}

TEST_F(RuntimeTest, getChromeProfile) {
	ASSERT_NOT_NULL(Berkelium::BerkeliumFactory::getDefaultRuntime()->getChromeProfile());
}

TEST_F(RuntimeTest, getChromiumProfile) {
	ASSERT_NOT_NULL(Berkelium::BerkeliumFactory::getDefaultRuntime()->getChromiumProfile());
}

TEST_F(RuntimeTest, createTemporaryProfile) {
	ASSERT_NOT_NULL(Berkelium::BerkeliumFactory::getDefaultRuntime()->createTemporaryProfile());
}

} // namespace
