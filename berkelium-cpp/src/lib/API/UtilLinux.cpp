// Copyright (c) 2012 The Berkelium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifdef LINUX

#include <Berkelium/API/Util.hpp>

#include <cstdlib>
#include <sys/select.h>

namespace Berkelium {

namespace Util {

std::string getEnv(const std::string& env, const std::string& defaultValue) {
	const char* ret = std::getenv(env.c_str());
	if(ret == NULL) {
		return defaultValue;
	}
	return ret;
}

void sleep(int32_t ms) {
	usleep(1000 * ms);
}

} // namespace Util

} // namespace Berkelium

#endif // LINUX
