// Copyright (c) 2012 The Berkelium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Berkelium/API/BerkeliumFactory.hpp>
#include <Berkelium/API/HostExecutable.hpp>
#include <Berkelium/API/Profile.hpp>

#include "gtest/gtest.h"
#include "test.h"

#include "boost/filesystem.hpp"

using namespace boost::filesystem;

namespace {

class ProfileTest : public ::testing::Test {
};

TEST_F(ProfileTest, create) {
	ASSERT_NOT_NULL(Berkelium::BerkeliumFactory::createTemporaryProfile());
}

TEST_F(ProfileTest, isInUse) {
	Berkelium::ProfileRef subject = Berkelium::BerkeliumFactory::createTemporaryProfile();
	ASSERT_NOT_NULL(subject);
	ASSERT_FALSE(subject->isInUse());
}

TEST_F(ProfileTest, isFound) {
	Berkelium::ProfileRef subject = Berkelium::BerkeliumFactory::createTemporaryProfile();
	ASSERT_NOT_NULL(subject);
	ASSERT_TRUE(subject->isFound());
}

TEST_F(ProfileTest, isSameVersion) {
	Berkelium::ProfileRef subject = Berkelium::BerkeliumFactory::createTemporaryProfile();
	ASSERT_NOT_NULL(subject);
	ASSERT_TRUE(subject->isSameVersion());
}

TEST_F(ProfileTest, isTooNew) {
	Berkelium::ProfileRef subject = Berkelium::BerkeliumFactory::createTemporaryProfile();
	ASSERT_NOT_NULL(subject);
	ASSERT_FALSE(subject->isTooNew());
}

TEST_F(ProfileTest, getApplicationName) {
	Berkelium::ProfileRef subject = Berkelium::BerkeliumFactory::createTemporaryProfile();
	ASSERT_NOT_NULL(subject);
	std::string str = subject->getApplicationName();
	ASSERT_EQ(0, str.compare("berkelium"));
}

TEST_F(ProfileTest, extended) {
	Berkelium::HostExecutableRef executable = Berkelium::BerkeliumFactory::forSystemInstalled();
	ASSERT_NOT_NULL(executable);
	Berkelium::ProfileRef profile = Berkelium::BerkeliumFactory::createTemporaryProfile();
	ASSERT_NOT_NULL(profile);
	path path = profile->getProfilePath();
	ASSERT_TRUE(exists(path));
	ASSERT_TRUE(profile->isFound());
	ASSERT_FALSE(profile->isInUse());
	Berkelium::InstanceRef instance = Berkelium::BerkeliumFactory::open(executable, profile);
	ASSERT_NOT_NULL(instance);
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
