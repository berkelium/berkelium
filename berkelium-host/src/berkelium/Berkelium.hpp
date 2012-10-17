// Copyright (c) 2012 The Berkelium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BERKELIUM_HPP_
#define BERKELIUM_HPP_
#pragma once

#include <string>

namespace Berkelium {

class WindowActions;
class PacketWriter;
class IpcSender;

class Berkelium {
public:
	static void init(const std::string&, const std::string&,const std::string&);

	static void lasyInit();

	static void destory();

	static bool isActive();

	static void addWindow(WindowActions*);

	static void removeWindow(WindowActions*);

	static PacketWriter* getPacketWriter();
};

} // namespace Berkelium

#endif // BERKELIUM_HPP_
