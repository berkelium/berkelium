// Copyright (c) 2012 The Berkelium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Berkelium/API/Util.hpp>
#include <Berkelium/API/Runtime.hpp>

#include <cstdlib>
#include <ctime>

namespace Berkelium {

namespace Util {

std::string randomId() {
	return randomId(8);
}

std::string randomId(int length) {
	static const std::string CHARS("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890");
	const int to = CHARS.size();
	std::string ret;
	static bool initialised = false;
	if(!initialised) {
		initialised = true;
		srand((unsigned)time(0));
	}
	for(int i = 0; i < length; i++) {
		ret += CHARS[std::rand() % to];
	}

	return ret;
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

void parseCommandLine(RuntimeRef runtime, int argc, char* argv[]) {
	std::string exec = getOption(argc, argv, "--berkelium-host=");
	if(!exec.empty()) {
		runtime->setDefaultExecutable(exec);
	}
}

} // namespace Util

} // namespace Berkelium
