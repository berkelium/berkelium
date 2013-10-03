// Copyright (c) 2012 The Berkelium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BERKELIUM_HOST_HPP_
#define BERKELIUM_HOST_HPP_
#pragma once

#include <string>
#include <memory>

#include <Berkelium/IPC/Ipc.hpp>

namespace Berkelium {

class BerkeliumHost {
public:
	static ProfileRef init(const std::string& dir, const std::string& id);

	static ProfileRef initDebug(const std::string& file);

	static void lasyInit();

	static bool isActive();

	static bool isDone();

	static void setDone();

	static bool update(int32_t ms);

	//static void CloseWindow();

	static Ipc::ChannelRef addWindow(void*);

	static void removeWindow(void*);

	static Ipc::ChannelRef addTab(void*);

	static void removeTab(void*);
};

} // namespace Berkelium

#endif // BERKELIUM_HOST_HPP_
