// Copyright (c) 2012 The Berkelium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifdef LINUX

#include "berkelium/Impl.hpp"

#include <cstdlib>
#include <sys/select.h>

namespace Berkelium {

namespace impl {

std::string getEnv(const std::string& env, const std::string& defaultValue) {
	const char* ret = std::getenv(env.c_str());
	if(ret == NULL) {
		return defaultValue;
	}
	return ret;
}

void sleep(int32_t ms) {
	struct timeval tv;
	tv.tv_sec = ms / 1000000L;
	tv.tv_usec = ms % 1000000L;
	select(0, NULL, NULL, NULL, &tv);
}

} // namespace impl

} // namespace Berkelium

#endif // LINUX
