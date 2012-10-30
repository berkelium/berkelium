// Copyright (c) 2012 The Berkelium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "berkelium/BerkeliumFactory.hpp"
#include "berkelium/HostExecutable.hpp"
#include "gtest/gtest.h"

#include "test.h"

namespace {

class HostExecutableTest : public ::testing::Test {
};

TEST_F(HostExecutableTest, create) {
	Berkelium::HostExecutableRef subject = Berkelium::BerkeliumFactory::forSystemInstalled();
	ASSERT_NOT_NULL(subject);
}

TEST_F(HostExecutableTest, getVersion) {
	Berkelium::HostExecutableRef subject = Berkelium::BerkeliumFactory::forSystemInstalled();
	ASSERT_NOT_NULL(subject);
	ASSERT_NOT_NULL(subject->getVersion());
}


} // namespace
