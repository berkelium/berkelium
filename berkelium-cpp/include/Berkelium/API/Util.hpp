// Copyright (c) 2012 The Berkelium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BERKELIUM_API_UTIL_HPP_
#define BERKELIUM_API_UTIL_HPP_
#pragma once

#include <Berkelium/API/Berkelium.hpp>

namespace Berkelium {

namespace Util {

void initRandom();
std::string randomId();
std::string randomId(int length);

int64_t currentTimeMillis();

std::string getEnv(const std::string&, const std::string& defaultValue);

LoggerRef createRootLogger(RuntimeRef runtime);

LoggerRef createRootLogger(LogDelegateRef target, RuntimeRef runtime);

RuntimeRef createRuntime(int argc, char* argv[]);

std::string getOption(int argc, char* argv[], const std::string& option);

std::string getOption(const std::string& cmdline, const std::string& option);

void parseCommandLine(RuntimeRef runtime, int argc, char* argv[]);

// Warning: you should not call this function,
// use runtime->update(ms) to give berkelium a change
// to process events in this time
void sleep(int32_t ms);

#ifdef WIN32
std::string ws2s(const std::wstring&);
std::wstring s2ws(const std::string&);
#endif

} // namespace Util

} // namespace Berkelium

#endif // BERKELIUM_API_UTIL_HPP_
