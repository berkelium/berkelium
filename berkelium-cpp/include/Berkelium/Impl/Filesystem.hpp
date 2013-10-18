// Copyright (c) 2012 The Berkelium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BERKELIUM_IMPL_FILESYSTEM_HPP_
#define BERKELIUM_IMPL_FILESYSTEM_HPP_
#pragma once

#include <string>
#include <vector>

namespace Berkelium {

namespace impl {

class Filesystem {
public:
	static std::string getTemp();

	static std::string createTemp();

	static std::vector<std::string> split(const std::string& arg);

	static std::string dirname(const std::string& arg);

	static std::string append(const std::string& dir, const std::string& append);

	static std::string append(const std::string& dir, const std::string& a1, const std::string& a2);

	static std::string append(const std::string& dir, const std::string& a1, const std::string& a2, const std::string& a3);

	static std::string absolute(const std::string& arg);

	static void createDirectory(const std::string& dir);

	static void createDirectories(const std::string& dir);

	static void createDirectoriesFor(const std::string& file);

	static void removeFile(const std::string& file);

	static void removeDir(const std::string& dir);

	static void removeEmptyDir(const std::string& dir);

	static bool exists(const std::string& arg);

	static bool readDirectory(const std::string& dir, std::vector<std::string>& content);

#if defined(LINUX)
	static bool readSymlink(const std::string& symlink, std::string& target);

	static bool createSymlink(const std::string& symlink, const std::string& target);
#endif
};

} // namespace impl

} // namespace Berkelium

#endif // BERKELIUM_IMPL_FILESYSTEM_HPP_
