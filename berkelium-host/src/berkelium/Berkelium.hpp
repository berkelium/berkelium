// Copyright (c) 2012 The Berkelium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BERKELIUM_HPP_
#define BERKELIUM_HPP_
#pragma once

#include <string>

#include <Berkelium/IPC/Ipc.hpp>

namespace Berkelium {

class BerkeliumTab;

class Berkelium {
public:
	static bool init(const std::string&, const std::string&);

	static void lasyInit();

	static void destory();

	static bool isActive();

	static ::Berkelium::impl::IpcRef addWindow(BerkeliumTab*);

	static void removeWindow(BerkeliumTab*);
};

} // namespace Berkelium

#endif // BERKELIUM_HPP_
