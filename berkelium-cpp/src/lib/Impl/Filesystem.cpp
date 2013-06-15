// Copyright (c) 2013 The Berkelium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Berkelium/Impl/Filesystem.hpp>

#include <sstream>
#include <stdlib.h>

namespace Berkelium {

namespace impl {

extern const char seperatorChar;
extern const std::string seperator;

std::string Filesystem::append(const std::string& dir, const std::string& append) {
	return dir + seperator + append;
}

std::string Filesystem::append(const std::string& dir, const std::string& a1, const std::string& a2) {
	return dir + seperator + a1 + seperator + a2;
}

std::string Filesystem::append(const std::string& dir, const std::string& a1, const std::string& a2, const std::string& a3) {
	return dir + seperator + a1 + seperator + a2 + seperator + a3;
}

std::vector<std::string> Filesystem::split(const std::string& arg) {
	std::vector<std::string> ret;
	std::stringstream ss(arg);
	std::string item;
	while(std::getline(ss, item, seperatorChar)) {
		if(!item.empty()) {
			ret.push_back(item);
		}
	}
	return ret;
}

std::string Filesystem::dirname(const std::string& arg) {
	size_t pos(arg.find_last_of(seperator));
	if(pos == std::string::npos) {
		return arg;
	}
	return arg.substr(0, pos);
}


} // namespace impl

} // namespace Berkelium
