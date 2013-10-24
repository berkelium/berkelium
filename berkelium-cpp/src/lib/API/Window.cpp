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
#include <Berkelium/IPC/LinkGroup.hpp>
#include <Berkelium/IPC/Link.hpp>
#include <Berkelium/IPC/ChannelGroup.hpp>
#include <Berkelium/Impl/Impl.hpp>
#include <Berkelium/Impl/Manager.hpp>

namespace Berkelium {

Window::Window() {
}

Window::~Window() {
}

namespace impl {

class WindowImpl : public Window, public Berkelium::Ipc::ChannelCallback {
BERKELIUM_IMPL_CLASS(Window)

private:
	WindowWRef self;
	InstanceRef instance;
	Ipc::ChannelGroupRef group;
	Ipc::ChannelRef send;
	Ipc::ChannelRef recv;
	Berkelium::Ipc::ChannelCallbackRef cb;
	std::list<TabWRef> tabs;
	const bool incognito;

public:
	WindowImpl(InstanceRef instance, Ipc::ChannelRef channel, bool incognito) :
		BERKELIUM_IMPL_CTOR3(Window, channel->getAlias(), instance),
		self(),
		instance(instance),
		group(channel->getGroup()),
		send(channel),
		recv(channel->getReverseChannel()),
		/*
		cb(),
		*/
		tabs(),
		incognito(incognito) {
		TRACE_OBJECT_NEW("WindowImpl");
	}

	virtual ~WindowImpl() {
		TRACE_OBJECT_DELETE("WindowImpl");
		getManager()->unregisterWindow();
	}

	virtual void onChannelDataReady(Ipc::ChannelRef channel, Ipc::MessageRef message) {
		switch(Ipc::CommandId cmd = message->get_cmd()) {
			default: {
				logger->error() << "Window: received unknown command '" << cmd << "'" << std::endl;
				break;
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
		Ipc::MessageRef message(Ipc::Message::create(logger));
		message->add_cmd(Ipc::CommandId::createTab);
		send->send(message);
		message = send->recv();
		int32_t id(message->get_32());
		logger->debug() << "created tab '" << id << "'" << std::endl;
		Ipc::ChannelRef x = group->getChannel(id, "tab");
		logger->debug() << "with channel '" << x->getId() << "'" << std::endl;
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
		impl->cb.reset(new Berkelium::Ipc::ChannelCallbackDelegate<Window, WindowImpl>(ret));
		channel->registerCallback(impl->cb);
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
