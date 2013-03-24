// Copyright (c) 2013 The Berkelium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Berkelium/Impl/Logger.hpp>

#include <cstring>
#include <iostream>

namespace Berkelium {

namespace Log {

std::string prefix = "\033[0;33mLib  ";

void setPrefix(const std::string& prefix) {
	Berkelium::Log::prefix = "\033[0;33m" + prefix + " ";
}

std::ostream& debug() {
	return std::cerr << prefix << "\033[1;34mDebug\033[1;33m:\033[0;37m ";
}

std::ostream& info() {
	return std::cerr << prefix << "\033[1;32mInfo \033[1;33m:\033[1;37m ";
}

std::ostream& warn() {
	return std::cerr << prefix << "\033[1;33mWarn \033[1;33m:\033[1;37m ";
}

std::ostream& error() {
	return std::cerr << prefix << "\033[1;31mError\033[1;33m:\033[1;37m ";
}

void systemError(const std::string& error) {
	Log::error() << error << ":" << strerror(errno) << std::endl;
}

} // namespace Log

} // namespace Berkelium
