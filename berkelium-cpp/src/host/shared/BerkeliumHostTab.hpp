// Copyright (c) 2013 The Berkelium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BERKELIUM_HOST_TAB_HPP_
#define BERKELIUM_HOST_TAB_HPP_
#pragma once

#include <Berkelium/IPC/Channel.hpp>
#include "BerkeliumHostWindow.hpp"

namespace Berkelium {

class BerkeliumHostTab;
typedef std::shared_ptr<BerkeliumHostTab> BerkeliumHostTabRef;
typedef std::weak_ptr<BerkeliumHostTab> BerkeliumHostTabWRef;

class BerkeliumHostTab : public Berkelium::Ipc::ChannelCallback {
protected:
	BerkeliumHostTab();

public:
	static BerkeliumHostTabRef createBerkeliumHostTab(BerkeliumHostWindowRef window, LoggerRef logger, Ipc::ChannelRef ipc);

	virtual ~BerkeliumHostTab() = 0;

	virtual void sendOnReady() = 0;

	virtual void sendOnPaint() = 0;
};

} // namespace Berkelium

#endif // BERKELIUM_HOST_TAB_HPP_
