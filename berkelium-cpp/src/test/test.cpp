#include <Berkelium/API/Util.hpp>
#include <Berkelium/API/BerkeliumFactory.hpp>

#include "gtest/gtest.h"

using Berkelium::Util::getOption;

GTEST_API_ int main(int argc, char **argv) {
	testing::InitGoogleTest(&argc, argv);

	Berkelium::Util::parseCommandLine(argc, argv);

	return RUN_ALL_TESTS();
}
