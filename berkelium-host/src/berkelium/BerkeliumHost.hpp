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

/*
class BerkeliumChromiumWindow;
typedef std::shared_ptr<BerkeliumChromiumWindow> BerkeliumChromiumWindowRef;
typedef std::weak_ptr<BerkeliumChromiumWindow> BerkeliumChromiumWindowWRef;

class BerkeliumChromiumTab;
typedef std::shared_ptr<BerkeliumChromiumTab> BerkeliumChromiumTabRef;
typedef std::weak_ptr<BerkeliumChromiumTab> BerkeliumChromiumTabWRef;
*/

class BerkeliumHost {
public:
	static bool init(const std::string&, const std::string&);

	static void lasyInit();

	static void destory();

	static bool isActive();

	//static void CloseWindow();

	static Ipc::ChannelRef addWindow(void*);

	static void removeWindow(void*);

	static Ipc::ChannelRef addTab(void*);

	static void removeTab(void*);
};

} // namespace Berkelium

#endif // BERKELIUM_HOST_HPP_
