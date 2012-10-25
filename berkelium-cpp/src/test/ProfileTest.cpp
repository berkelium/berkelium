#include "berkelium/HostExecutableFactory.hpp"
#include "berkelium/HostExecutable.hpp"
#include "berkelium/Profile.hpp"
#include "gtest/gtest.h"

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
	ASSERT_TRUE(subject); // not NULL
}

TEST_F(ProfileTest, isInUse) {
	ASSERT_FALSE(subject->isInUse()); // not NULL
}

TEST_F(ProfileTest, isFound) {
	ASSERT_TRUE(subject->isFound()); // not NULL
}

TEST_F(ProfileTest, isSameVersion) {
	ASSERT_TRUE(subject->isSameVersion()); // not NULL
}

TEST_F(ProfileTest, isTooNew) {
	ASSERT_FALSE(subject->isTooNew()); // not NULL
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
	Berkelium::InstanceRef ref = subject->open();
	ASSERT_FALSE(ref); // NULL
}

} // namespace
