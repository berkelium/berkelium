// Copyright (c) 2012 The Berkelium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Berkelium/API/Runtime.hpp>
#include <Berkelium/API/BerkeliumFactory.hpp>
#include <Berkelium/API/Instance.hpp>
#include <Berkelium/API/HostDelegate.hpp>
#include <Berkelium/API/Logger.hpp>
#include <Berkelium/API/Util.hpp>
#include "gtest/gtest.h"

#include "test.h"

namespace {

using Berkelium::InstanceRef;

class TestHostDelegate : public Berkelium::HostDelegate {
public:
	int onCrashedCount;
	int onClosedCount;
	int onPingCount;

	TestHostDelegate() :
		Berkelium::HostDelegate(),
		onCrashedCount(0),
		onClosedCount(0),
		onPingCount(0) {
	}

	virtual ~TestHostDelegate() {
	}

	virtual void onCrashed(InstanceRef instance) {
		onCrashedCount++;
	}

	virtual void onClosed(InstanceRef instance) {
		onClosedCount++;
	}

	virtual void onPing(InstanceRef instance) {
		onPingCount++;
	}
};

class InstanceTest : public ::testing::Test {
	DEFINE_LOGGER(InstanceTest);
};

void createInstance(Berkelium::LoggerRef logger, Berkelium::InstanceRef& ret) {
	Berkelium::RuntimeRef runtime(Berkelium::BerkeliumFactory::getDefaultRuntime());
	logger->debug() << "creating host executable..." << std::endl;
	Berkelium::HostExecutableRef host = runtime->forSystemInstalled();
	ASSERT_NOT_NULL(host);

	logger->debug() << "creating profile..." << std::endl;
	Berkelium::ProfileRef profile = runtime->createTemporaryProfile();
	ASSERT_NOT_NULL(profile);

	logger->debug() << "launching berkelium host executable..." << std::endl;
	ret = runtime->open(host, profile);
	ASSERT_NOT_NULL(ret);

}

TEST_F(InstanceTest, create) {
	USE_LOGGER(create);

	InstanceRef subject;
	createInstance(logger, subject);
	ASSERT_NOT_NULL(subject);
}

TEST_F(InstanceTest, ping) {
	USE_LOGGER(ping);

	InstanceRef subject;
	createInstance(logger, subject);
	ASSERT_NOT_NULL(subject);

	std::shared_ptr<TestHostDelegate> thd(new TestHostDelegate());
	subject->addHostDelegate(thd);

	logger->info() << "waiting up to 30s..." << std::endl;
	for(int i = 0; i < 30000; i += 100) {
		subject->getRuntime()->update(100);
		// wait for at least two pings
		if(thd->onPingCount >= 2) {
			logger->info() << "ping received, test ok!" << std::endl;
			return;
		}
	}
	logger->info() << "no ping received within 30s, test failed!" << std::endl;
	ASSERT_TRUE(false);
}

// TODO not working yet
TEST_F(InstanceTest, crash) {
	USE_LOGGER(crash);

	InstanceRef subject;
	createInstance(logger, subject);
	ASSERT_NOT_NULL(subject);

	std::shared_ptr<TestHostDelegate> thd(new TestHostDelegate());
	subject->addHostDelegate(thd);

	logger->info() << "waiting to to 30s..." << std::endl;
	for(int i = 0; i < 30000; i += 100) {
		// normaly, sleep should not be used.
		// we use it here to let the host think the library process (we)
		// are busy. this will cause host shutdown and lead to onCrashed called.
		Berkelium::Util::sleep(100);
		if(thd->onCrashedCount != 0) {
			logger->info() << "host terminated, test ok!" << std::endl;
			return;
		}
	}
	logger->info() << "host not terminated within 30s, test failed!" << std::endl;
	ASSERT_TRUE(false);
}

} // namespace
