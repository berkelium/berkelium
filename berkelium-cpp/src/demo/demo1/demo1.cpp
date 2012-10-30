// Copyright (c) 2012 The Berkelium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <iostream>

#include "berkelium/Profile.hpp"
#include "berkelium/HostVersion.hpp"
#include "berkelium/HostExecutable.hpp"
#include "berkelium/BerkeliumFactory.hpp"

int main(int argc, char* argv[])
{
	std::cout << "berkelium demo application..." << std::endl;
	Berkelium::HostExecutableRef host = Berkelium::BerkeliumFactory::forSystemInstalled();

	Berkelium::HostVersionRef version = host->getVersion();
	std::cout << "host version string: " << version->getVersionString() << std::endl;

	/*
	Berkelium::ProfileRef profile = Berkelium::BerkeliumFactory::createTemporaryProfile();
	*/
	Berkelium::ProfileRef profile = Berkelium::BerkeliumFactory::forChromeProfile();
	std::cout << "profile application: " << profile->getApplicationName() << std::endl;
	std::cout << "profile name: " << profile->getProfileName() << std::endl;
	std::cout << "profile path: " << profile->getProfilePath() << std::endl;
	std::cout << "profile is found: " << profile->isFound() << std::endl;
	std::cout << "profile in use: " << profile->isInUse() << std::endl;

	return 0;
}
