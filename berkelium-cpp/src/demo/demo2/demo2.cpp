// Copyright (c) 2012 The Berkelium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Berkelium/API/BerkeliumFactory.hpp>
#include <Berkelium/API/Instance.hpp>
#include <Berkelium/API/Util.hpp>
#include <Berkelium/API/Window.hpp>
#include <Berkelium/API/Tab.hpp>

// TODO: not the best way...
#define BERKELIUM_CPP_IMPL
#include <Berkelium/Impl/Logger.hpp>
#undef BERKELIUM_CPP_IMPL

int main(int argc, char* argv[])
{
	Berkelium::Util::parseCommandLine(argc, argv);

	Berkelium::Log::info() << "berkelium demo application..." << std::endl;
	Berkelium::HostExecutableRef host = Berkelium::BerkeliumFactory::forSystemInstalled();
	if(!host) {
		Berkelium::Log::info() << "berkelium host executable not found!" << std::endl;
		return 1;
	}
	Berkelium::ProfileRef profile = Berkelium::BerkeliumFactory::createTemporaryProfile();
	Berkelium::Log::info() << "starting berkelium browser..." << std::endl;
	Berkelium::InstanceRef instance = Berkelium::BerkeliumFactory::open(host, profile);
	if(!instance) {
		Berkelium::Log::info() << "berkelium browser can not be started!" << std::endl;
		return 1;
	}
	Berkelium::Log::info() << "berkelium browser is running!" << std::endl;

	Berkelium::WindowRef win(instance->createWindow(false));
	Berkelium::TabRef tab(win->createTab());

	Berkelium::Log::info() << "waiting 10s..." << std::endl;

	for(int i = 0; i < 10000; i += 100) {
		instance->internalUpdate();
		Berkelium::Util::sleep(100);
	}

	Berkelium::Log::info() << "shutting down browser..." << std::endl;
	instance.reset();
	profile.reset();
	host.reset();
	Berkelium::Log::info() << "done!" << std::endl;
}
