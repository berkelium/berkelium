// Copyright (c) 2012 The Berkelium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Berkelium/API/Util.hpp>
#include <Berkelium/API/Profile.hpp>
#include <Berkelium/API/HostVersion.hpp>
#include <Berkelium/API/HostExecutable.hpp>
#include <Berkelium/API/BerkeliumFactory.hpp>

// TODO: not the best way...
#define BERKELIUM_CPP_IMPL
#include <Berkelium/Impl/Logger.hpp>
#undef BERKELIUM_CPP_IMPL

void dumpProfile(const std::string& name, Berkelium::ProfileRef profile)
{
	if(!profile) {
		Berkelium::Log::error() << name << " profile not found!" << std::endl;
		return;
	}
	Berkelium::Log::info() << "profile application: " << profile->getApplicationName() << std::endl;
	Berkelium::Log::info() << "profile path: " << profile->getProfilePath() << std::endl;
	Berkelium::Log::info() << "profile is found: " << profile->isFound() << std::endl;
	Berkelium::Log::info() << "profile in use: " << profile->isInUse() << std::endl;
}

int main(int argc, char* argv[])
{
	Berkelium::Util::parseCommandLine(argc, argv);

	Berkelium::Log::info() << "berkelium demo application..." << std::endl;
	Berkelium::HostExecutableRef host = Berkelium::BerkeliumFactory::forSystemInstalled();

	Berkelium::HostVersionRef version = host->getVersion();
	Berkelium::Log::info() << "host version string: " << version->getVersionString() << std::endl;

	dumpProfile("temporary", Berkelium::BerkeliumFactory::createTemporaryProfile());
	dumpProfile("Chrome", Berkelium::BerkeliumFactory::getChromeProfile());
	dumpProfile("Chromium", Berkelium::BerkeliumFactory::getChromiumProfile());
	dumpProfile("Berkelium", Berkelium::BerkeliumFactory::forProfile("berkelium"));
}
