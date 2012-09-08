// Copyright (c) 2012 The Berkelium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BERKELIUM_HPP_
#define BERKELIUM_HPP_
#pragma once

#include <string>

namespace Berkelium {

class Berkelium {
public:
	static void init();

	static void lasyInit();

	static void destory();

	static bool isActive();

	static void send(std::string msg);
};

} // namespace Berkelium

#endif // BERKELIUM_HPP_
