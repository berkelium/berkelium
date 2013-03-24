// Copyright (c) 2012 The Berkelium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifdef LINUX

#include <Berkelium/API/Util.hpp>
#include <Berkelium/API/BerkeliumFactory.hpp>

#include <cstdlib>
#include <sys/select.h>

namespace Berkelium {

namespace Util {

std::string getEnv(const std::string& env, const std::string& defaultValue) {
	const char* ret = std::getenv(env.c_str());
	if(ret == NULL) {
		return defaultValue;
	}
	return ret;
}

void sleep(int32_t ms) {
	usleep(1000 * ms);
}

std::string getOption(int argc, char* argv[], const std::string& option) {
	for(int i = 0; i < argc; i++) {
		std::string tmp(argv[i]);
		if(tmp.compare(0, option.length(), option) == 0) {
			return tmp.substr(option.length());
		}
	}
	return "";
}

void parseCommandLine(int argc, char* argv[]) {
	std::string exec = getOption(argc, argv, "--berkelium-host=");
	if(!exec.empty()) {
		Berkelium::BerkeliumFactory::setDefaultExecutable(exec);
	}
}

} // namespace Util

} // namespace Berkelium

#endif // LINUX
