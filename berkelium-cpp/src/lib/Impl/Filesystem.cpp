// Copyright (c) 2013 The Berkelium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Berkelium/Impl/Filesystem.hpp>

#include <stdlib.h>

namespace Berkelium {

namespace impl {

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

} // namespace impl

} // namespace Berkelium
