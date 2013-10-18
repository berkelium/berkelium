// Copyright (c) 2013 The Berkelium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifdef OS_UNIX

#include <Berkelium/API/Util.hpp>
#include <Berkelium/Impl/Filesystem.hpp>

#include <limits.h>
#include <stdlib.h>
#include <memory.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>

namespace Berkelium {

namespace impl {

extern const char seperatorChar = '/';
extern const std::string seperator = "/";

std::string Filesystem::append(const std::string& dir, const std::string& append) {
	return dir + seperator + append;
}

std::string Filesystem::absolute(const std::string& arg) {
	char ret[PATH_MAX];
	memset(ret, 0, PATH_MAX);
	if(realpath(arg.c_str(), ret) < 0) {
		// TODO error handling?
		return arg;
	}
	return ret;
}

std::string Filesystem::getTemp() {
	return append("/tmp", "berkelium." + Util::getEnv("USER", "User"));
}

void Filesystem::createDirectory(const std::string& dir) {
	mkdir(dir.c_str(), 448); // mode 700
}

void Filesystem::removeFile(const std::string& file) {
	unlink(file.c_str());
}

void Filesystem::removeEmptyDir(const std::string& dir) {
	rmdir(dir.c_str());
}

bool Filesystem::exists(const std::string& arg) {
	struct stat s;
	if(stat(arg.c_str(), &s) < 0) {
		return false;
	}
	return true;
}

bool Filesystem::readDirectory(const std::string& dir, std::vector<std::string>& content) {
	DIR* d = opendir(dir.c_str());
	if(!d) {
		return false;
	}
	dirent* e;
	while (0 != (e = readdir(d))) {
		std::string name(e->d_name);
		if(name == "." || name == "..") {
			continue;
		}
		content.push_back(name);
	}
	closedir(d);
	return true;
}

bool Filesystem::readSymlink(const std::string& symlink, std::string& target) {
	char buf[PATH_MAX];
	memset(buf, 0, PATH_MAX);
	if(readlink(symlink.c_str(), buf, PATH_MAX) < 0) {
		return false;
	}
	target = buf;
	return true;
}

bool Filesystem::createSymlink(const std::string& symlink, const std::string& target) {
	if(::symlink(target.c_str(), symlink.c_str()) < 0) {
		return false;
	}
	return true;
}

} // namespace impl

} // namespace Berkelium

#endif // OS_UNIX
