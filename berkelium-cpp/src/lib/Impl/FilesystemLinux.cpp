// Copyright (c) 2013 The Berkelium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifdef OS_UNIX

#include <Berkelium/Impl/Filesystem.hpp>

#include <limits.h>
#include <stdlib.h>
#include <sys/stat.h>

namespace Berkelium {

namespace impl {

extern const std::string seperator = "/";

std::string Filesystem::absolute(const std::string& arg) {
	char ret[PATH_MAX];
	if(realpath(arg.c_str(), ret) == 0) {
		// TODO
		return "";
	}
	return ret;
}

void Filesystem::createDirectories(const std::string& dir) {
	mkdir(dir.c_str(), 700);
}

void Filesystem::createDirectoriesFor(const std::string& file) {
}

void Filesystem::removeFile(const std::string& file) {
}

void Filesystem::removeDir(const std::string& dir) {
}

bool Filesystem::exists(const std::string& arg) {
	return false;
}

bool Filesystem::readDirectory(const std::string& dir, std::vector<std::string>& content) {
	return false;
}

bool Filesystem::readSymlink(const std::string& symlink, std::string& target) {
	return false;
}

bool Filesystem::createSymlink(const std::string& symlink, const std::string& target) {
	return false;
}

} // namespace impl

} // namespace Berkelium

#endif // OS_UNIX
