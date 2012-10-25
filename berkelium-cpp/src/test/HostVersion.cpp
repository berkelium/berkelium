#include "berkelium/HostExecutableFactory.hpp"
#include "berkelium/HostExecutable.hpp"
#include "berkelium/HostVersion.hpp"
#include "gtest/gtest.h"

namespace {

class HostVersionTest : public ::testing::Test {
public:
	Berkelium::HostExecutableRef executable;
	Berkelium::HostVersionRef subject;

	HostVersionTest() {
		executable = Berkelium::HostExecutableFactory::forSystemInstalled();
		subject = executable->getVersion();
	}
};

TEST_F(HostVersionTest, create) {
	ASSERT_TRUE(executable); // not NULL
	ASSERT_TRUE(subject); // not NULL
}

TEST_F(HostVersionTest, getVersionString) {
	std::string str = subject->getVersionString();
	ASSERT_EQ(0, str.compare("0.0.0.0"));
}

TEST_F(HostVersionTest, getMajor) {
	ASSERT_EQ(0, subject->getMajor());
}

TEST_F(HostVersionTest, getMinor) {
	ASSERT_EQ(0, subject->getMajor());
}

TEST_F(HostVersionTest, getBuild) {
	ASSERT_EQ(0, subject->getBuild());
}

TEST_F(HostVersionTest, getPatch) {
	ASSERT_EQ(0, subject->getPatch());
}

TEST_F(HostVersionTest, isMinVersion) {
	ASSERT_FALSE(subject->isMinVersion(""));
}

} // namespace
