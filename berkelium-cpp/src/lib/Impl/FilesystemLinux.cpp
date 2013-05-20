// Copyright (c) 2013 The Berkelium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifdef LINUX

#include <Berkelium/Impl/Filesystem.hpp>

#include <boost/filesystem.hpp>

namespace Berkelium {

namespace impl {

std::string Filesystem::append(const std::string& dir, const std::string& append) {
	boost::filesystem::path a(dir);
	boost::filesystem::path b(append);
	return boost::filesystem::path(a / b).string();
}

std::string Filesystem::absolute(const std::string& arg) {
	return boost::filesystem::absolute(boost::filesystem::path(arg)).string();
}

void Filesystem::createDirectories(const std::string& dir) {
	boost::filesystem::create_directories(boost::filesystem::path(dir));
}

void Filesystem::createDirectoriesFor(const std::string& file) {
	createDirectories(boost::filesystem::path(file).parent_path().string());
}

void Filesystem::removeFile(const std::string& file) {
	boost::filesystem::remove(boost::filesystem::path(file));
}

void Filesystem::removeDir(const std::string& dir) {
	boost::filesystem::remove_all(boost::filesystem::path(dir));
}

bool Filesystem::exists(const std::string& arg) {
	return boost::filesystem::exists(boost::filesystem::path(arg));
}

bool Filesystem::readDirectory(const std::string& dir, std::vector<std::string>& content) {
	boost::filesystem::path tmp(dir);
	if(!boost::filesystem::is_directory(dir)) {
		return false;
	}
	for(boost::filesystem::directory_iterator itr(tmp); itr != boost::filesystem::directory_iterator(); ++itr) {
		content.push_back(itr->path().string());
	}
	return true;
}

bool Filesystem::readSymlink(const std::string& symlink, std::string& target) {
	boost::system::error_code ec;
	boost::filesystem::path read(boost::filesystem::read_symlink(boost::filesystem::path(symlink), ec));
	if(ec) {
		return false;
	}
	target = read.string();
	return true;
}

bool Filesystem::createSymlink(const std::string& symlink, const std::string& target) {
	boost::system::error_code ec;
	boost::filesystem::create_symlink(boost::filesystem::path(target), boost::filesystem::path(symlink), ec);
	if(ec) {
		return false;
	}
	return true;
}

} // namespace impl

} // namespace Berkelium

#endif // LINUX
