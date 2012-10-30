// Copyright (c) 2012 The Berkelium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "berkelium/BerkeliumFactory.hpp"
#include "berkelium/HostExecutable.hpp"
#include "gtest/gtest.h"

#include "test.h"

namespace {

class BerkeliumFactoryTest : public ::testing::Test {
};

TEST_F(BerkeliumFactoryTest, forExecutable) {
	ASSERT_NOT_NULL(Berkelium::BerkeliumFactory::forExecutable(""));
}

TEST_F(BerkeliumFactoryTest, forSystemInstalled) {
	ASSERT_NOT_NULL(Berkelium::BerkeliumFactory::forSystemInstalled());
}

TEST_F(BerkeliumFactoryTest, forProfile) {
	ASSERT_NOT_NULL(Berkelium::BerkeliumFactory::forProfile(""));
}

TEST_F(BerkeliumFactoryTest, getChromeProfile) {
	ASSERT_NOT_NULL(Berkelium::BerkeliumFactory::getChromeProfile());
}

TEST_F(BerkeliumFactoryTest, getChromiumProfile) {
	ASSERT_NOT_NULL(Berkelium::BerkeliumFactory::getChromiumProfile());
}

TEST_F(BerkeliumFactoryTest, createTemporaryProfile) {
	ASSERT_NOT_NULL(Berkelium::BerkeliumFactory::createTemporaryProfile());
}

} // namespace
