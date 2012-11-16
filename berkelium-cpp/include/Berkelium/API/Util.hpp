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

std::string getEnv(const std::string&, const std::string& defaultValue);

#ifdef WIN32
std::string ws2s(const std::wstring&);
std::wstring s2ws(const std::string&);
#endif

} // namespace Util

} // namespace Berkelium

#endif // BERKELIUM_API_UTIL_HPP_
