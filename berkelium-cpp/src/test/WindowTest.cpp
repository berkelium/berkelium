// Copyright (c) 2012 The Berkelium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Berkelium/API/Runtime.hpp>
#include <Berkelium/API/BerkeliumFactory.hpp>
#include <Berkelium/API/Instance.hpp>
#include <Berkelium/API/Window.hpp>
#include <Berkelium/API/Tab.hpp>
#include <Berkelium/API/Logger.hpp>
#include "gtest/gtest.h"

#include <list>

#include "test.h"

namespace {

using Berkelium::InstanceRef;
using Berkelium::WindowRef;
using Berkelium::TabRef;

class WindowTest : public ::testing::Test {
	DEFINE_LOGGER(IpcMessageTest);
};

void createWindow(Berkelium::LoggerRef logger, WindowRef& ret, bool incognito) {
	Berkelium::RuntimeRef runtime(Berkelium::BerkeliumFactory::getDefaultRuntime());
	logger->debug() << "creating host executable..." << std::endl;
	Berkelium::HostExecutableRef host = runtime->forSystemInstalled();
	ASSERT_NOT_NULL(host);

	logger->debug() << "creating profile..." << std::endl;
	Berkelium::ProfileRef profile = runtime->createTemporaryProfile();
	ASSERT_NOT_NULL(profile);

	logger->debug() << "launching berkelium host executable..." << std::endl;
	Berkelium::InstanceRef instance = runtime->open(host, profile);
	ASSERT_NOT_NULL(instance);

	logger->debug() << "creating window..." << std::endl;
	int old = instance->getWindowCount();
	ret = instance->createWindow(incognito);
	ASSERT_EQ(old + 1, instance->getWindowCount());
	ASSERT_EQ(0, ret->getTabCount());
}

TEST_F(WindowTest, create) {
	USE_LOGGER(create);
	WindowRef subject;
	createWindow(logger, subject, false);
	ASSERT_NOT_NULL(subject);
}

TEST_F(WindowTest, createIncognito) {
	USE_LOGGER(create);
	WindowRef subject;
	createWindow(logger, subject, true);
	ASSERT_NOT_NULL(subject);
}

TEST_F(WindowTest, createSecondProcessWindow) {
	USE_LOGGER(createSecondProcessWindow);
	WindowRef subject1;
	createWindow(logger, subject1, false);
	ASSERT_NOT_NULL(subject1);
	WindowRef subject2;
	createWindow(logger, subject2, false);
	ASSERT_NOT_NULL(subject2);
	ASSERT_NOT_SAME(subject1->getInstance(), subject2->getInstance());
}

TEST_F(WindowTest, createSecondWindow) {
	USE_LOGGER(createSecondWindow);
	WindowRef subject1;
	createWindow(logger, subject1, false);
	ASSERT_NOT_NULL(subject1);

	logger->debug() << "creating second window..." << std::endl;
	WindowRef subject2 = subject1->getInstance()->createWindow(false);
	ASSERT_NOT_NULL(subject2);
	ASSERT_SAME(subject1->getInstance(), subject2->getInstance());
}

TEST_F(WindowTest, createWindows) {
	USE_LOGGER(createWindows);
	WindowRef first;
	createWindow(logger, first, false);
	ASSERT_NOT_NULL(first);
	InstanceRef instance = first->getInstance();

	std::vector<WindowRef> windows;
	for(int i = 0; i < 10; i++) {
		logger->debug() << "creating window" << i << "..." << std::endl;
		WindowRef other = instance->createWindow(false);
		ASSERT_NOT_NULL(other);
		ASSERT_SAME(instance, other	->getInstance());
		windows.push_back(other);
	}
}

TEST_F(WindowTest, createDeleteRandomWindows) {
	USE_LOGGER(createDeleteRandomWindows);
	WindowRef first;
	createWindow(logger, first, false);
	ASSERT_NOT_NULL(first);
	InstanceRef instance = first->getInstance();

	std::list<WindowRef> windows;
	for(int i = 0; i < 20; i++) {
		bool del = !windows.empty();
		if(del) {
			del = rand() % 2 == 0;
		}
		if(del) {
			int pos = rand() % windows.size();
			for (std::list<WindowRef>::iterator it = windows.begin(); it != windows.end(); it++) {
				if(pos == 0) {
					logger->info() << "removing window..." << std::endl;
					windows.remove(*it);
					break;
				}
				pos--;
			}
		} else {
			logger->debug() << "creating window..." << std::endl;
			WindowRef other = instance->createWindow(rand() % 2 == 0);
			ASSERT_NOT_NULL(other);
			ASSERT_SAME(instance, other->getInstance());
			windows.push_back(other);
		}
	}
	windows.clear();
}

TEST_F(WindowTest, createTab) {
	USE_LOGGER(createTab);
	WindowRef window;
	createWindow(logger, window, false);
	ASSERT_NOT_NULL(window);

	ASSERT_EQ(0, window->getTabCount());
	TabRef tab = window->createTab();
	ASSERT_NOT_NULL(tab);
	ASSERT_EQ(1, window->getTabCount());
}

TEST_F(WindowTest, createMultipleTabs) {
	USE_LOGGER(createMultipleTabs);
	WindowRef window;
	createWindow(logger, window, false);
	ASSERT_NOT_NULL(window);

	std::list<TabRef> tabs;
	for(int i = 0; i < 10; i++) {
		ASSERT_EQ(i, window->getTabCount());
		TabRef tab = window->createTab();
		ASSERT_NOT_NULL(tab);
		tabs.push_back(tab);
		ASSERT_EQ(i + 1, window->getTabCount());
	}
	tabs.clear();
	ASSERT_EQ(0, window->getTabCount());
}

TEST_F(WindowTest, free1) {
	USE_LOGGER(free1);
	WindowRef window;
	createWindow(logger, window, false);
	ASSERT_NOT_NULL(window);

	TabRef tab = window->createTab();
	int count = tab->getWindow().use_count();
	window.reset();
	ASSERT_EQ(count - 1, tab->getWindow().use_count());
}

TEST_F(WindowTest, free2) {
	USE_LOGGER(free2);
	WindowRef window;
	createWindow(logger, window, false);
	ASSERT_NOT_NULL(window);

	ASSERT_EQ(0, window->getTabCount());
	TabRef tab = window->createTab();
	ASSERT_NOT_NULL(tab);
	ASSERT_EQ(1, window->getTabCount());
	tab.reset();
	ASSERT_EQ(0, window->getTabCount());
}

} // namespace
