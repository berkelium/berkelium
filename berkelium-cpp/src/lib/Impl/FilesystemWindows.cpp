// Copyright (c) 2013 The Berkelium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifdef OS_WINDOWS

#include <Berkelium/Impl/Filesystem.hpp>

#include <windows.h>
#include <tchar.h>

namespace Berkelium {

namespace impl {

extern const std::string seperator = "\\";

#define BUFSIZE 4096

std::string Filesystem::absolute(const std::string& arg) {
	TCHAR buffer[BUFSIZE];
	if(GetFullPathName(arg.c_str(), BUFSIZE, buffer, NULL) == 0) {
		// TODO
		return "";
	}
	return buffer;
}

std::string Filesystem::getTemp() {
	return append(Util::getEnv("TEMP", "C:\\WINDOWS\\TEMP"), "berkelium." + Util::getEnv("USERNAME", "User"));
}

void Filesystem::createDirectories(const std::string& dir) {
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

} // namespace impl

} // namespace Berkelium

#endif // OS_WINDOWS
