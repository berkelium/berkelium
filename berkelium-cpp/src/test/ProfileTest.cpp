// Copyright (c) 2012 The Berkelium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "berkelium/HostExecutableFactory.hpp"
#include "berkelium/HostExecutable.hpp"
#include "berkelium/Profile.hpp"

#include "gtest/gtest.h"
#include "test.h"

#include "boost/filesystem.hpp"

using namespace boost::filesystem;

namespace {

class ProfileTest : public ::testing::Test {
};

TEST_F(ProfileTest, create) {
	Berkelium::HostExecutableRef executable = Berkelium::HostExecutableFactory::forSystemInstalled();
	ASSERT_NOT_NULL(executable);
	Berkelium::ProfileRef subject = executable->createTemporaryProfile();
	ASSERT_NOT_NULL(subject);
}

TEST_F(ProfileTest, isInUse) {
	Berkelium::HostExecutableRef executable = Berkelium::HostExecutableFactory::forSystemInstalled();
	ASSERT_NOT_NULL(executable);
	Berkelium::ProfileRef subject = executable->createTemporaryProfile();
	ASSERT_NOT_NULL(subject);
	ASSERT_FALSE(subject->isInUse());
}

TEST_F(ProfileTest, isFound) {
	Berkelium::HostExecutableRef executable = Berkelium::HostExecutableFactory::forSystemInstalled();
	ASSERT_NOT_NULL(executable);
	Berkelium::ProfileRef subject = executable->createTemporaryProfile();
	ASSERT_NOT_NULL(subject);
	ASSERT_FALSE(subject->isFound());
}

TEST_F(ProfileTest, isSameVersion) {
	Berkelium::HostExecutableRef executable = Berkelium::HostExecutableFactory::forSystemInstalled();
	ASSERT_NOT_NULL(executable);
	Berkelium::ProfileRef subject = executable->createTemporaryProfile();
	ASSERT_NOT_NULL(subject);
	ASSERT_TRUE(subject->isSameVersion());
}

TEST_F(ProfileTest, isTooNew) {
	Berkelium::HostExecutableRef executable = Berkelium::HostExecutableFactory::forSystemInstalled();
	ASSERT_NOT_NULL(executable);
	Berkelium::ProfileRef subject = executable->createTemporaryProfile();
	ASSERT_NOT_NULL(subject);
	ASSERT_FALSE(subject->isTooNew());
}

TEST_F(ProfileTest, getApplicationName) {
	Berkelium::HostExecutableRef executable = Berkelium::HostExecutableFactory::forSystemInstalled();
	ASSERT_NOT_NULL(executable);
	Berkelium::ProfileRef subject = executable->createTemporaryProfile();
	ASSERT_NOT_NULL(subject);
	std::string str = subject->getApplicationName();
	ASSERT_EQ(0, str.compare("berkelium"));
}

TEST_F(ProfileTest, getProfileName) {
	Berkelium::HostExecutableRef executable = Berkelium::HostExecutableFactory::forSystemInstalled();
	ASSERT_NOT_NULL(executable);
	Berkelium::ProfileRef subject = executable->forProfile("berkelium");
	ASSERT_NOT_NULL(subject);
	std::string str = subject->getProfileName();
	ASSERT_EQ(0, str.compare("Default"));
}

/*
TEST_F(ProfileTest, getProfilePath) {
	// TODO?
}
*/

TEST_F(ProfileTest, open) {
	Berkelium::HostExecutableRef executable = Berkelium::HostExecutableFactory::forSystemInstalled();
	ASSERT_NOT_NULL(executable);
	Berkelium::ProfileRef subject = executable->createTemporaryProfile();
	ASSERT_NOT_NULL(subject);
	ASSERT_NULL(subject->open());
}

TEST_F(ProfileTest, extended) {
	Berkelium::HostExecutableRef executable = Berkelium::HostExecutableFactory::forSystemInstalled();
	ASSERT_NOT_NULL(executable);
	Berkelium::ProfileRef profile = executable->createTemporaryProfile();
	ASSERT_NOT_NULL(profile);
	path path = profile->getProfilePath();
	ASSERT_FALSE(exists(path));
	ASSERT_FALSE(profile->isFound());
	ASSERT_FALSE(profile->isInUse());
	Berkelium::InstanceRef instance = profile->open();
	ASSERT_NOT_NULL(instance);
	ASSERT_TRUE(exists(path));
	ASSERT_TRUE(profile->isFound());
	ASSERT_TRUE(profile->isInUse());
	instance.reset();
	ASSERT_NULL(instance);
	ASSERT_TRUE(exists(path));
	ASSERT_TRUE(profile->isFound());
	ASSERT_FALSE(profile->isInUse());
	profile.reset();
	ASSERT_FALSE(exists(path));
}

} // namespace
