#include "berkelium/HostExecutableFactory.hpp"
#include "berkelium/HostExecutable.hpp"
#include "gtest/gtest.h"

namespace {

class HostExecutableTest : public ::testing::Test {
public:
	Berkelium::HostExecutableRef subject;

	HostExecutableTest() {
		subject = Berkelium::HostExecutableFactory::forSystemInstalled();
	}
};

TEST_F(HostExecutableTest, create) {
	ASSERT_TRUE(subject); // not NULL
}

TEST_F(HostExecutableTest, getVersion) {
	ASSERT_TRUE(subject->getVersion()); // not NULL
}

TEST_F(HostExecutableTest, forProfile1) {
	ASSERT_TRUE(subject->forProfile("")); // not NULL
}

TEST_F(HostExecutableTest, forProfile2) {
	ASSERT_TRUE(subject->forProfile("", "")); // not NULL
}

TEST_F(HostExecutableTest, createTemporaryProfile) {
	ASSERT_TRUE(subject->createTemporaryProfile()); // not NULL
}

} // namespace
