// Copyright (c) 2013 The Berkelium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Berkelium/Impl/Filesystem.hpp>
#include <Berkelium/API/Util.hpp>

#include <sstream>
#include <stdlib.h>

namespace Berkelium {

namespace impl {

extern const char seperatorChar;
extern const std::string seperator;

std::string Filesystem::createTemp() {
	std::string ret(append(getTemp(), Util::randomId()));
	createDirectories(ret);
	return ret;
}

std::string Filesystem::append(const std::string& dir, const std::string& a1, const std::string& a2) {
	return append(append(dir, a1), a2);
}

std::string Filesystem::append(const std::string& dir, const std::string& a1, const std::string& a2, const std::string& a3) {
	return append(append(dir, a1, a2), a3);
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

void Filesystem::createDirectories(const std::string& dir) {
	std::vector<std::string> s(split(dir));
	std::string full;
	for(unsigned i = 0; i < s.size(); i++) {
		full = append(full, s[i]);
		createDirectory(full);
	}
}

void Filesystem::createDirectoriesFor(const std::string& file) {
	createDirectories(dirname(file));
}

void Filesystem::removeDir(const std::string& dir) {
    std::vector<std::string> c;
    if(readDirectory(dir, c)) {
        for(unsigned i = 0; i < c.size(); i++) {
            std::string name(append(dir, c[i]));
            removeDir(name);
            removeFile(name);
        }
        removeEmptyDir(dir.c_str());
    }
}

} // namespace impl

} // namespace Berkelium
