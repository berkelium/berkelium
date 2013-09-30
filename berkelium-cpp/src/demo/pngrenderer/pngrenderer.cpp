// Copyright (c) 2012 The Berkelium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Berkelium/API/Runtime.hpp>
#include <Berkelium/API/Profile.hpp>
#include <Berkelium/API/HostExecutable.hpp>
#include <Berkelium/API/BerkeliumFactory.hpp>
#include <Berkelium/API/Instance.hpp>
#include <Berkelium/API/Util.hpp>
#include <Berkelium/API/Window.hpp>
#include <Berkelium/API/Logger.hpp>

using Berkelium::BerkeliumFactory;
using Berkelium::HostExecutableRef;
using Berkelium::ProfileRef;
using Berkelium::InstanceRef;
using Berkelium::WindowRef;

int main(int argc, char* argv[])
{
	Berkelium::RuntimeRef runtime(Berkelium::Util::createRuntime(argc, argv));
	Berkelium::LoggerRef logger(Berkelium::Util::createRootLogger(runtime));

	logger->info() << "running pngrenderer..." << std::endl;
	HostExecutableRef host(runtime->forSystemInstalled());

	logger->debug() << "creating profile..." << std::endl;
	ProfileRef profile(runtime->createTemporaryProfile());

	logger->debug() << "launching berkelium host executable..." << std::endl;
	InstanceRef instance(runtime->open(host, profile));

	logger->debug() << "creating window..." << std::endl;
	WindowRef window = instance->createWindow(false);

	logger->debug() << "window: " << window.get() << std::endl;

	window->getTabCount();

	logger->debug() << "tested!" << std::endl;

	for(int i = 0; i < 2000; i += 100) {
		runtime->update(100);
	}

	logger->debug() << "closing window..." << std::endl;
	window.reset();

	logger->debug() << "closing berkelium host executable..." << std::endl;
	instance.reset();

	logger->debug() << "closing profile..." << std::endl;
	profile.reset();

	logger->debug() << "closing host..." << std::endl;
	host.reset();

	logger->info() << "pngrenderer finished!" << std::endl;
}
