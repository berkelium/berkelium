// Copyright (c) 2013 The Berkelium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BERKELIUM_HOST_WINDOW_HPP_
#define BERKELIUM_HOST_WINDOW_HPP_
#pragma once

#include <Berkelium/IPC/Channel.hpp>

namespace Berkelium {

class BerkeliumHostWindow;
typedef std::shared_ptr<BerkeliumHostWindow> BerkeliumHostWindowRef;
typedef std::weak_ptr<BerkeliumHostWindow> BerkeliumHostWindowWRef;

class BerkeliumHostWindow : public Berkelium::Ipc::ChannelCallback {
protected:
	BerkeliumHostWindow();

public:
	static BerkeliumHostWindowRef createBerkeliumHostWindow(LoggerRef logger, Ipc::ChannelRef ipc, bool incognito);

	virtual ~BerkeliumHostWindow() = 0;

	virtual void* getNative() = 0;
};

} // namespace Berkelium

#endif // BERKELIUM_HOST_WINDOW_HPP_
