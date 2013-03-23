#include <Berkelium/API/Util.hpp>
#include <Berkelium/API/BerkeliumFactory.hpp>

#include "gtest/gtest.h"

using Berkelium::Util::getOption;

GTEST_API_ int main(int argc, char **argv) {
	testing::InitGoogleTest(&argc, argv);

	std::string exec = getOption(argc, argv, "--berkelium-host=");
	if(!exec.empty()) {
		Berkelium::BerkeliumFactory::setDefaultExecutable(exec);
	}

	return RUN_ALL_TESTS();
}
