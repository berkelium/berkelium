// Copyright (c) 2012 The Berkelium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BERKELIUM_API_UTIL_HPP_
#define BERKELIUM_API_UTIL_HPP_
#pragma once

#include <Berkelium/API/Berkelium.hpp>

namespace Berkelium {

namespace Util {

std::string randomId();
std::string randomId(int length);

void sleep(int32_t ms);

int64_t currentTimeMillis(LoggerRef logger);

std::string getEnv(const std::string&, const std::string& defaultValue);

LoggerRef createRootLogger(RuntimeRef runtime, std::ostream& out);

#ifdef LINUX
RuntimeRef createRuntime(int argc, char* argv[]);

std::string getOption(int argc, char* argv[], const std::string& option);

void parseCommandLine(RuntimeRef runtime, int argc, char* argv[]);
#endif

#ifdef WIN32
std::string ws2s(const std::wstring&);
std::wstring s2ws(const std::string&);
#endif

} // namespace Util

} // namespace Berkelium

#endif // BERKELIUM_API_UTIL_HPP_
