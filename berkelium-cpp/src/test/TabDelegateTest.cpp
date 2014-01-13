// Copyright (c) 2013 The Berkelium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Berkelium/API/Runtime.hpp>
#include <Berkelium/API/BerkeliumFactory.hpp>
#include <Berkelium/API/Instance.hpp>
#include <Berkelium/API/Window.hpp>
#include <Berkelium/API/Tab.hpp>
#include <Berkelium/API/TabDelegate.hpp>
#include <Berkelium/API/Logger.hpp>
#include <Berkelium/API/Util.hpp>
#include <Berkelium/Impl/Impl.hpp>
#include "gtest/gtest.h"

#include "test.h"

namespace {

using Berkelium::TabRef;

class TestTabDelegate : public Berkelium::TabDelegate {
public:
	int onClosedCount;
	int onPaintCount;
	int onReadyCount;

	TestTabDelegate() :
		Berkelium::TabDelegate(),
		onClosedCount(0),
		onPaintCount(0),
		onReadyCount(0) {
	}

	virtual ~TestTabDelegate() {
	}

	virtual void onClosed(TabRef tab) {
		onClosedCount++;
	}

	virtual void onTitleChanged(TabRef tab, const std::string& title) {
	}

	virtual void onPaint(TabRef tab) {
		onPaintCount++;
	}

	virtual void onPaintDone(TabRef tab, Berkelium::RectRef rect) {
	}

	virtual void onReady(TabRef tab) {
		onReadyCount++;
	}
};

class TabDelegateTest : public ::testing::Test {
	DEFINE_LOGGER(TabDelegateTest);
};

void createTab(Berkelium::LoggerRef logger, Berkelium::TabRef& ret) {
	Berkelium::RuntimeRef runtime(Berkelium::BerkeliumFactory::getDefaultRuntime());
	logger->debug() << "creating host executable..." << std::endl;
	Berkelium::HostExecutableRef host = runtime->forSystemInstalled();
	ASSERT_NOT_NULL(host);

	logger->debug() << "creating profile..." << std::endl;
	Berkelium::ProfileRef profile = runtime->createTemporaryProfile();
	ASSERT_NOT_NULL(profile);

	logger->debug() << "launching berkelium host executable..." << std::endl;
	Berkelium::InstanceRef instance(runtime->open(host, profile));
	ASSERT_NOT_NULL(instance);

	Berkelium::WindowRef window(instance->createWindow(false));

	ret = window->createTab();

	ASSERT_NOT_NULL(ret);

}

TEST_F(TabDelegateTest, create) {
	USE_LOGGER(create);

	TabRef subject;
	createTab(logger, subject);
	ASSERT_NOT_NULL(subject);
}

TEST_F(TabDelegateTest, onReady) {
	USE_LOGGER(onReady);

	TabRef subject;
	createTab(logger, subject);
	ASSERT_NOT_NULL(subject);

	std::shared_ptr<TestTabDelegate> thd(new TestTabDelegate());
	subject->addTabDelegate(thd);

	Berkelium::RuntimeRef runtime(subject->getRuntime());

	logger->info() << "waiting up to 30s..." << std::endl;
	for(int i = 0; i < 30000; i += 100) {
		runtime->update(100);
		ASSERT_EQ(0, thd->onClosedCount);
		if(thd->onReadyCount != 0) {
			logger->info() << "tab is ready, test ok!" << std::endl;
			return;
		}
	}
	logger->info() << "tab not ready within 30s, test failed!" << std::endl;
	ASSERT_TRUE(false);
}

TEST_F(TabDelegateTest, onPaint) {
	USE_LOGGER(onPaint);

	TabRef subject;
	createTab(logger, subject);
	ASSERT_NOT_NULL(subject);

	std::shared_ptr<TestTabDelegate> ttd(new TestTabDelegate());
	subject->addTabDelegate(ttd);

	Berkelium::RuntimeRef runtime(subject->getRuntime());

	logger->info() << "waiting up to 30s..." << std::endl;
	for(int i = 0; i < 30000; i += 100) {
		runtime->update(100);
		ASSERT_EQ(0, ttd->onClosedCount);
		if(ttd->onPaintCount != 0) {
			logger->info() << "tab painted, test ok!" << std::endl;
			return;
		}
	}
	logger->info() << "tab not painted within 30s, test failed!" << std::endl;
	ASSERT_TRUE(false);
}

} // namespace
