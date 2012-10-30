// Copyright (c) 2012 The Berkelium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <iostream>

#include "berkelium/Profile.hpp"
#include "berkelium/HostVersion.hpp"
#include "berkelium/HostExecutable.hpp"
#include "berkelium/BerkeliumFactory.hpp"

void dumpProfile(const std::string& name, Berkelium::ProfileRef profile)
{
	if(!profile) {
		std::cout << name << " profile not found!" << std::endl;
		return;
	}
	std::cout << "profile application: " << profile->getApplicationName() << std::endl;
	std::cout << "profile path: " << profile->getProfilePath() << std::endl;
	std::cout << "profile is found: " << profile->isFound() << std::endl;
	std::cout << "profile in use: " << profile->isInUse() << std::endl;
}

int main(int argc, char* argv[])
{
	std::cout << "berkelium demo application..." << std::endl;
	Berkelium::HostExecutableRef host = Berkelium::BerkeliumFactory::forSystemInstalled();
	if(!host) {
		std::cout << "berkelium host executable not found!" << std::endl;
		return 1;
	}
	Berkelium::ProfileRef profile = Berkelium::BerkeliumFactory::createTemporaryProfile();
	std::cout << "starting berkelium browser..." << std::endl;
	Berkelium::InstanceRef instance = Berkelium::BerkeliumFactory::open(host, profile);
	if(!instance) {
		std::cout << "berkelium browser can not be started!" << std::endl;
		return 1;
	}
	std::cout << "berkelium browser is running!" << std::endl;
}
