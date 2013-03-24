// Copyright (c) 2012 The Berkelium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Berkelium/API/BerkeliumFactory.hpp>
#include <Berkelium/API/Instance.hpp>
#include <Berkelium/API/Window.hpp>
#include <Berkelium/Impl/Logger.hpp>
#include "gtest/gtest.h"

#include "test.h"

namespace {

using Berkelium::WindowRef;

class WindowTest : public ::testing::Test {
};

WindowRef createWindow() {
	Berkelium::Log::debug() << "creating host executable..." << std::endl;
	Berkelium::HostExecutableRef host = Berkelium::BerkeliumFactory::forSystemInstalled();

	Berkelium::Log::debug() << "creating profile..." << std::endl;
	Berkelium::ProfileRef profile = Berkelium::BerkeliumFactory::createTemporaryProfile();

	Berkelium::Log::debug() << "launching berkelium host executable..." << std::endl;
	Berkelium::InstanceRef instance = Berkelium::BerkeliumFactory::open(host, profile);

	Berkelium::Log::debug() << "creating window..." << std::endl;
	return instance->createWindow(false);
}

TEST_F(WindowTest, create) {
	WindowRef subject = createWindow();
	ASSERT_NOT_NULL(subject);
}

TEST_F(WindowTest, createSecondProcessWindow) {
	WindowRef subject1 = createWindow();
	ASSERT_NOT_NULL(subject1);
	WindowRef subject2 = createWindow();
	ASSERT_NOT_NULL(subject2);
}

TEST_F(WindowTest, createSecondWindow) {
	WindowRef subject1 = createWindow();
	ASSERT_NOT_NULL(subject1);
	Berkelium::Log::debug() << "creating second window..." << std::endl;
	WindowRef subject2 = subject1->getInstance()->createWindow(false);
	ASSERT_NOT_NULL(subject2);
}

} // namespace
