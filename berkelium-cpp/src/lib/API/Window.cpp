// Copyright (c) 2012 The Berkelium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Berkelium/API/Window.hpp>
#include <Berkelium/IPC/Message.hpp>
#include <Berkelium/Impl/Impl.hpp>
#include <Berkelium/Impl/Logger.hpp>

#include <iostream>

namespace Berkelium {

Window::Window() {
}

Window::~Window() {
}

namespace impl {

class WindowImpl : public Window {
	InstanceRef instance;
	Ipc::ChannelRef channel;
	const bool incognito;
	Ipc::MessageRef message;

public:
	WindowImpl(InstanceRef instance, Ipc::ChannelRef channel, bool incognito) :
		instance(instance),
		channel(channel),
		incognito(incognito),
		message(Ipc::Message::create()) {
		Log::debug() << "new Window" << std::endl;
	}

	virtual ~WindowImpl() {
		Log::debug() << "delete Window" << std::endl;
	}

	virtual int32_t getTabCount() {
		/*
		Log::debug() << "new Window start" << std::endl;
		channel->recv(message);
		Log::debug() << "new Window done" << std::endl;
		*/
		return 0;
	}

	virtual TabList getTabList()  {
		return TabList();
	}

	virtual TabRef createTab()  {
		return TabRef();
	}

	virtual InstanceRef getInstance() {
		return instance;
	}

	virtual void moveTo(TabRef tab, int32_t index)  {
	}

	virtual bool isIncognito()  {
		return incognito;
	}
};

WindowRef newWindow(InstanceRef instance, Ipc::ChannelRef channel, bool incognito) {
	return WindowRef(new WindowImpl(instance, channel, incognito));
}

} // namespace impl

} // namespace Berkelium
