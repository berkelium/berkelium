// Copyright (c) 2012 The Berkelium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Berkelium/API/Util.hpp>

#include <cstdlib>
#include <ctime>

namespace Berkelium {

namespace Util {

std::string randomId() {
	return randomId(8);
}

std::string randomId(int length) {
	static const std::string CHARS("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890");
	const int to = CHARS.size();
	std::string ret;
	static bool initialised = false;
	if(!initialised) {
		initialised = true;
		srand((unsigned)time(0));
	}
	for(int i = 0; i < length; i++) {
		ret += CHARS[std::rand() % to];
	}

	return ret;
}

} // namespace Util

} // namespace Berkelium
