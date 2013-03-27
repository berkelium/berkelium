// Copyright (c) 2013 The Berkelium Authors. All rights reserved.
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

Tab::Tab() {
}

Tab::~Tab() {
}

namespace impl {

class TabImpl : public Tab {
private:
	TabWRef self;
	WindowRef parent;

public:
	TabImpl(WindowRef parent) :
		Tab(),
		self(),
		parent(parent) {
	}

	virtual ~TabImpl() {
	}

	virtual void close() {
	}

	virtual void sync() {
	}

	virtual TabDelegateRef getTabDelegate() {
		return TabDelegateRef();
	}

	virtual void addTabDelegate(TabDelegateRef delegate) {
	}

	virtual void removeTabDelegate(TabDelegateRef delegate) {
	}

	virtual WindowRef getWindow() {
		return parent;
	}

	virtual void resize(int32_t width, int32_t height) {
	}

	virtual void navigateTo(const std::string& url) {
	}

	TabRef getSelf() 	{
		return self.lock();
	}

	static TabRef newTab(WindowRef window) {
		TabImpl* impl = new TabImpl(window);
		TabRef ret(impl);
		impl->self = ret;
		return ret;
	}
};

TabRef newTab(WindowRef window) {
	return TabImpl::newTab(window);
}

} // namespace impl

} // namespace Berkelium
