// Copyright (c) 2012 The Berkelium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "berkelium/HostExecutableFactory.hpp"
#include "berkelium/HostExecutable.hpp"
#include "berkelium/Profile.hpp"
#include "gtest/gtest.h"

#include "test.h"

namespace {

class ProfileTest : public ::testing::Test {
public:
	Berkelium::HostExecutableRef executable;
	Berkelium::ProfileRef subject;

	ProfileTest() {
		executable = Berkelium::HostExecutableFactory::forSystemInstalled();
		subject = executable->createTemporaryProfile();
	}
};

TEST_F(ProfileTest, create) {
	ASSERT_NOT_NULL(subject);
}

TEST_F(ProfileTest, isInUse) {
	ASSERT_FALSE(subject->isInUse());
}

TEST_F(ProfileTest, isFound) {
	ASSERT_TRUE(subject->isFound());
}

TEST_F(ProfileTest, isSameVersion) {
	ASSERT_TRUE(subject->isSameVersion());
}

TEST_F(ProfileTest, isTooNew) {
	ASSERT_FALSE(subject->isTooNew());
}

TEST_F(ProfileTest, getApplicationName) {
	std::string str = subject->getApplicationName();
	ASSERT_EQ(0, str.compare("todo"));
}

TEST_F(ProfileTest, getProfileName) {
	std::string str = subject->getProfileName();
	ASSERT_EQ(0, str.compare("todo"));
}

TEST_F(ProfileTest, getProfilePath) {
	std::string str = subject->getProfilePath();
	ASSERT_EQ(0, str.compare("todo"));
}

TEST_F(ProfileTest, open) {
	ASSERT_NULL(subject->open());
}

} // namespace
