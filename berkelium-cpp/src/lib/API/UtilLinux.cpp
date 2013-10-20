// Copyright (c) 2012 The Berkelium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifdef LINUX

#include <Berkelium/API/Util.hpp>
#include <Berkelium/API/Logger.hpp>
#include <Berkelium/Impl/Impl.hpp>

#include <sys/time.h>
#include <cstdlib>
#include <sys/select.h>
#include <unistd.h>
#include <sys/types.h>

namespace Berkelium {

namespace Util {

void initRandom() {
	static bool initialised = false;
	if(!initialised) {
		initialised = true;
		timeval t;
		gettimeofday( &t, NULL);
		srand((unsigned)time(0) + getpid() + t.tv_usec);
	}
}

int64_t currentTimeMillis() {
	timeval t;
	if(gettimeofday( &t, NULL) == -1)
		Berkelium::impl::bk_error("gettimeofday");
	return int64_t(t.tv_sec) * 1000 + int64_t(t.tv_usec) / 1000;
}

std::string getEnv(const std::string& env, const std::string& defaultValue) {
	const char* ret = std::getenv(env.c_str());
	if(ret == NULL) {
		return defaultValue;
	}
	return ret;
}

void sleep(int32_t ms) {
	usleep(ms * 1000);
}

} // namespace Util

} // namespace Berkelium

#endif // LINUX
