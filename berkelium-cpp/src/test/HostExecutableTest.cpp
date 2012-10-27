// Copyright (c) 2012 The Berkelium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "berkelium/HostExecutableFactory.hpp"
#include "berkelium/HostExecutable.hpp"
#include "gtest/gtest.h"

#include "test.h"

namespace {

class HostExecutableTest : public ::testing::Test {
public:
	Berkelium::HostExecutableRef subject;

	HostExecutableTest() {
		subject = Berkelium::HostExecutableFactory::forSystemInstalled();
	}
};

TEST_F(HostExecutableTest, create) {
	ASSERT_NOT_NULL(subject);
}

TEST_F(HostExecutableTest, getVersion) {
	ASSERT_NOT_NULL(subject->getVersion());
}

TEST_F(HostExecutableTest, forProfile1) {
	ASSERT_NOT_NULL(subject->forProfile(""));
}

TEST_F(HostExecutableTest, forProfile2) {
	ASSERT_NOT_NULL(subject->forProfile("", ""));
}

TEST_F(HostExecutableTest, createTemporaryProfile) {
	ASSERT_NOT_NULL(subject->createTemporaryProfile());
}

} // namespace
