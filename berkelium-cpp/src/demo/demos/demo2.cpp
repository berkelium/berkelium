// Copyright (c) 2012 The Berkelium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Berkelium/API/Runtime.hpp>
#include <Berkelium/API/BerkeliumFactory.hpp>
#include <Berkelium/API/Instance.hpp>
#include <Berkelium/API/Util.hpp>
#include <Berkelium/API/Window.hpp>
#include <Berkelium/API/Tab.hpp>
#include <Berkelium/API/Logger.hpp>

int main(int argc, char* argv[])
{
	Berkelium::RuntimeRef runtime(Berkelium::Util::createRuntime(argc, argv));
	Berkelium::LoggerRef logger(Berkelium::Util::createRootLogger(runtime));

	logger->info() << "berkelium demo application..." << std::endl;
	Berkelium::HostExecutableRef host(runtime->forSystemInstalled());
	if(!host) {
		logger->info() << "berkelium host executable not found!" << std::endl;
		return 1;
	}
	Berkelium::ProfileRef profile(runtime->createTemporaryProfile());
	logger->info() << "starting berkelium browser..." << std::endl;
	Berkelium::InstanceRef instance(runtime->open(host, profile));
	if(!instance) {
		logger->info() << "berkelium browser can not be started!" << std::endl;
		return 1;
	}
	logger->info() << "berkelium browser is running!" << std::endl;

	Berkelium::WindowRef win(instance->createWindow(false));
	Berkelium::TabRef tab(win->createTab());

	logger->info() << "waiting 10s..." << std::endl;

	for(int i = 0; i < 10000; i += 100) {
		instance->internalUpdate();
		Berkelium::Util::sleep(100);
	}

	logger->info() << "shutting down browser..." << std::endl;
	instance.reset();
	profile.reset();
	host.reset();
	logger->info() << "done!" << std::endl;
}
