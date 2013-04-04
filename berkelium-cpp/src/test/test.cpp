#include <Berkelium/API/Runtime.hpp>
#include <Berkelium/API/Util.hpp>
#include <Berkelium/API/BerkeliumFactory.hpp>

#include "gtest/gtest.h"

using Berkelium::Util::getOption;

GTEST_API_ int main(int argc, char **argv) {
	testing::InitGoogleTest(&argc, argv);

	Berkelium::RuntimeRef runtime(Berkelium::BerkeliumFactory::createRuntime());
	Berkelium::Util::parseCommandLine(runtime, argc, argv);

	return RUN_ALL_TESTS();
}
