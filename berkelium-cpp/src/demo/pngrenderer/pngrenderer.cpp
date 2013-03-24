// Copyright (c) 2012 The Berkelium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Berkelium/API/Profile.hpp>
#include <Berkelium/API/HostExecutable.hpp>
#include <Berkelium/API/BerkeliumFactory.hpp>
#include <Berkelium/API/Instance.hpp>
#include <Berkelium/API/Util.hpp>
#include <Berkelium/API/Window.hpp>

// TODO: not the best way...
#define BERKELIUM_CPP_IMPL
#include <Berkelium/Impl/Logger.hpp>
#undef BERKELIUM_CPP_IMPL

using Berkelium::BerkeliumFactory;
using Berkelium::HostExecutableRef;
using Berkelium::ProfileRef;
using Berkelium::InstanceRef;
using Berkelium::WindowRef;

int main(int argc, char* argv[])
{
	Berkelium::Util::parseCommandLine(argc, argv);

	Berkelium::Log::info() << "running pngrenderer..." << std::endl;
	HostExecutableRef host = BerkeliumFactory::forSystemInstalled();

	Berkelium::Log::debug() << "creating profile..." << std::endl;
	ProfileRef profile = BerkeliumFactory::createTemporaryProfile();

	Berkelium::Log::debug() << "launching berkelium host executable..." << std::endl;
	InstanceRef instance = BerkeliumFactory::open(host, profile);

	Berkelium::Log::debug() << "creating window..." << std::endl;
	WindowRef window = instance->createWindow(false);

	Berkelium::Log::debug() << "window: " << window.get() << std::endl;

	window->getTabCount();

	Berkelium::Log::debug() << "tested!" << std::endl;

	for(int i = 0; i < 2000; i += 100) {
		instance->update();
		Berkelium::Util::sleep(100);
	}

	Berkelium::Log::debug() << "closing window..." << std::endl;
	window.reset();

	Berkelium::Log::debug() << "closing berkelium host executable..." << std::endl;
	instance.reset();

	Berkelium::Log::debug() << "closing profile..." << std::endl;
	profile.reset();

	Berkelium::Log::debug() << "closing host..." << std::endl;
	host.reset();

	Berkelium::Log::info() << "pngrenderer finished!" << std::endl;
}
