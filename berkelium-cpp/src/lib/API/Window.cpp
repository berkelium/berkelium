// Copyright (c) 2012 The Berkelium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Berkelium/API/Util.hpp>
#include <Berkelium/API/Window.hpp>
#include <Berkelium/API/Tab.hpp>
#include <Berkelium/IPC/Message.hpp>
#include <Berkelium/Impl/Impl.hpp>
#include <Berkelium/Impl/Logger.hpp>

#include <list>

namespace Berkelium {

Window::Window() {
}

Window::~Window() {
}

namespace impl {

class WindowImpl : public Window {
private:
	WindowWRef self;
	InstanceRef instance;
	Ipc::ChannelRef channel;
	const bool incognito;
	std::list<TabRef> tabs;
	Ipc::MessageRef message;

public:
	WindowImpl(InstanceRef instance, Ipc::ChannelRef channel, bool incognito) :
		Window(),
		self(),
		instance(instance),
		channel(channel),
		incognito(incognito),
		tabs(),
		message(Ipc::Message::create()) {
	}

	virtual ~WindowImpl() {
	}

	virtual int32_t getTabCount() {
		return tabs.size();
	}

	virtual TabList getTabList()  {
		TabList ret;
		std::copy(tabs.begin(), tabs.end(), ret.begin());
		return ret;
	}

	virtual TabRef createTab()  {
		message->reset();
		message->add_str("openTab");
		channel->send(message);
		channel->recv(message);
		TabRef ret(newTab(getSelf()));
		tabs.push_back(ret);
		return ret;
	}

	virtual InstanceRef getInstance() {
		return instance;
	}

	virtual void moveTo(TabRef tab, int32_t index)  {
	}

	virtual bool isIncognito()  {
		return incognito;
	}

	const WindowRef getSelf() 	{
		return self.lock();
	}

	static WindowRef newWindow(InstanceRef instance, Ipc::ChannelRef channel, bool incognito) {
		WindowImpl* impl = new WindowImpl(instance, channel, incognito);
		WindowRef ret(impl);
		impl->self = ret;
		return ret;
	}
};

WindowRef newWindow(InstanceRef instance, Ipc::ChannelRef channel, bool incognito) {
	return WindowImpl::newWindow(instance, channel, incognito);
}

} // namespace impl

} // namespace Berkelium
