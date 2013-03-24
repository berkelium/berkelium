// Copyright (c) 2012 The Berkelium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <iostream>

#include <Berkelium/API/Profile.hpp>
#include <Berkelium/API/HostExecutable.hpp>
#include <Berkelium/API/BerkeliumFactory.hpp>
#include <Berkelium/API/Instance.hpp>
#include <Berkelium/API/Util.hpp>
#include <Berkelium/API/Window.hpp>

using Berkelium::BerkeliumFactory;
using Berkelium::HostExecutableRef;
using Berkelium::ProfileRef;
using Berkelium::InstanceRef;
using Berkelium::WindowRef;

int main(int argc, char* argv[])
{
	Berkelium::Util::parseCommandLine(argc, argv);

	std::cout << "running pngrenderer..." << std::endl;
	HostExecutableRef host = BerkeliumFactory::forSystemInstalled();

	std::cout << "creating profile..." << std::endl;
	ProfileRef profile = BerkeliumFactory::createTemporaryProfile();

	std::cout << "launching berkelium host executable..." << std::endl;
	InstanceRef instance = BerkeliumFactory::open(host, profile);

	std::cout << "creating window..." << std::endl;
	WindowRef window = instance->createWindow(false);

	std::cout << "window: " << window.get() << std::endl;

	window->getTabCount();

	std::cout << "tested!" << std::endl;

	for(int i = 0; i < 2000; i += 100) {
		instance->update();
		Berkelium::Util::sleep(100);
	}

	std::cout << "closing window..." << std::endl;
	window.reset();

	std::cout << "closing berkelium host executable..." << std::endl;
	instance.reset();

	std::cout << "closing profile..." << std::endl;
	profile.reset();

	std::cout << "closing host..." << std::endl;
	host.reset();

	std::cout << "pngrenderer finished!" << std::endl;
}
