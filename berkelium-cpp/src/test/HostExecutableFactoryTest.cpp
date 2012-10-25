#include "berkelium/HostExecutableFactory.hpp"
#include "gtest/gtest.h"

namespace {

class HostExecutableFactoryTest : public ::testing::Test {
};

TEST_F(HostExecutableFactoryTest, forExecutable) {
	Berkelium::HostExecutableRef he = Berkelium::HostExecutableFactory::forExecutable("");
	ASSERT_TRUE(he); // not NULL
}

TEST_F(HostExecutableFactoryTest, forSystemInstalled) {
	Berkelium::HostExecutableRef he = Berkelium::HostExecutableFactory::forSystemInstalled();
	ASSERT_TRUE(he); // not NULL
}

} // namespace
