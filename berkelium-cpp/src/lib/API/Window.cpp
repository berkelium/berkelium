// Copyright (c) 2012 The Berkelium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Berkelium/API/Util.hpp>
#include <Berkelium/API/Instance.hpp>
#include <Berkelium/API/Window.hpp>
#include <Berkelium/API/Tab.hpp>
#include <Berkelium/API/Runtime.hpp>
#include <Berkelium/API/Logger.hpp>
#include <Berkelium/IPC/Message.hpp>
#include <Berkelium/Impl/Impl.hpp>
#include <Berkelium/Impl/Manager.hpp>
#include <Berkelium/Impl/BerkeliumCallback.hpp>

namespace Berkelium {

Window::Window() {
}

Window::~Window() {
}

namespace impl {

class WindowImpl : public Window, public InternalUpdate {
BERKELIUM_IMPL_CLASS(Window)

private:
	WindowWRef self;
	InstanceRef instance;
	Ipc::ChannelRef send;
	Ipc::ChannelRef recv;
	Ipc::MessageRef message;
	Berkelium::Ipc::ChannelCallbackRef cb;
	std::list<TabWRef> tabs;
	const bool incognito;

public:
	WindowImpl(InstanceRef instance, Ipc::ChannelRef channel, bool incognito) :
		BERKELIUM_IMPL_CTOR3(Window, channel->getName(), instance),
		self(),
		instance(instance),
		send(channel),
		recv(channel->getReverseChannel()),
		message(channel->getMessage()),
		cb(),
		tabs(),
		incognito(incognito) {
	}

	virtual ~WindowImpl() {
		getManager()->unregisterWindow();
	}

	void internalUpdate() {
		if(!recv->isEmpty()) {
			recv->recv(message);
			if(message->length() == 0) {
				// only an ack..
			} else {
				switch(Ipc::CommandId cmd = message->get_cmd()) {
					default: {
						logger->error() << "Window: received unknown command '" << cmd << "'" << std::endl;
						break;
					}
					/*
					case Ipc::CommandId::onReady: {
						msg->reset();
						msg->add_cmd(Ipc::CommandId::navigate);
						msg->add_str("http://heise.de/");
						Log::debug() << "sending navigate to heise.de!" << std::endl;
						send->send(msg);
						break;
					}
					*/
				}
			}
		}
	}

	virtual int32_t getTabCount() {
		cleanupTabs();
		return tabs.size();
	}

	virtual TabListRef getTabList()  {
		cleanupTabs();
		TabListRef ret(new TabList());
		for(std::list<TabWRef>::iterator it = tabs.begin(); it != tabs.end(); it++) {
			ret->push_back(it->lock());
		}
		return ret;
	}

	virtual TabRef createTab()  {
		cleanupTabs();
		message->reset();
		message->add_cmd(Ipc::CommandId::createTab);
		send->send(message);
		send->recv(message);
		std::string id(message->get_str());
		logger->debug() << "created tab '" << id << "'" << std::endl;
		Ipc::ChannelRef x = send->getSubChannel(id);
		logger->debug() << "with channel '" << x->getName() << "'" << std::endl;
		TabRef ret(newTab(getSelf(), x));
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

	static bool tabIsExpired(TabWRef& tab) {
		return tab.expired();
	}

	void cleanupTabs() {
		tabs.remove_if(tabIsExpired);
	}

	const WindowRef getSelf() 	{
		return self.lock();
	}

	static WindowRef newWindow(InstanceRef instance, Ipc::ChannelRef channel, bool incognito) {
		WindowImpl* impl = new WindowImpl(instance, channel, incognito);
		WindowRef ret(impl);
		impl->self = ret;
		impl->cb.reset(new BerkeliumCallback<Window, WindowImpl>(ret));
		impl->manager->registerWindow(ret);
		return ret;
	}
};

ManagerRef getManager(Window* window)
{
	if(!window) {
		bk_error("getManager(Window* = null)");
		return ManagerRef();
	}
	WindowImpl* impl = (WindowImpl*)window;
	return impl->getManager();
}

WindowRef newWindow(InstanceRef instance, Ipc::ChannelRef channel, bool incognito) {
	return WindowImpl::newWindow(instance, channel, incognito);
}

} // namespace impl

} // namespace Berkelium
