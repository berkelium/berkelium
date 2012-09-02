// Copyright (c) 2012 The Berkelium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BERKELIUM_H_
#define BERKELIUM_H_
#pragma once

#include <string>

namespace Berkelium {

class Berkelium {
public:
	static void init();

	static void destory();

	static void send(std::string msg);
};

} // namespace Berkelium

#endif // BERKELIUM_H_
