// Copyright (c) 2012 The Berkelium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Berkelium/API/Runtime.hpp>
#include <Berkelium/API/Util.hpp>
#include <Berkelium/API/Profile.hpp>
#include <Berkelium/API/HostVersion.hpp>
#include <Berkelium/API/HostExecutable.hpp>
#include <Berkelium/API/BerkeliumFactory.hpp>
#include <Berkelium/API/Logger.hpp>

void dumpProfile(Berkelium::LoggerRef logger, const std::string& name, Berkelium::ProfileRef profile)
{
	if(!profile) {
		logger->error() << name << " profile not found!" << std::endl;
		return;
	}
	logger->info() << "profile application: " << profile->getApplicationName() << std::endl;
	logger->debug() << "profile path: " << profile->getProfilePath() << std::endl;
	logger->debug() << "profile is found: " << profile->isFound() << std::endl;
	logger->debug() << "profile in use: " << profile->isInUse() << std::endl;
}

int main(int argc, char* argv[])
{
	Berkelium::RuntimeRef runtime(Berkelium::Util::createRuntime(argc, argv));
	Berkelium::LoggerRef logger(Berkelium::Util::createRootLogger(runtime, std::cout));

	logger->info() << "berkelium demo application..." << std::endl;
	Berkelium::HostExecutableRef host(runtime->forSystemInstalled());

	Berkelium::HostVersionRef version(host->getVersion());
	logger->info() << "host version string: " << version->getVersionString() << std::endl;

	dumpProfile(logger, "temporary", runtime->createTemporaryProfile());
	dumpProfile(logger, "Chrome", runtime->getChromeProfile());
	dumpProfile(logger, "Chromium", runtime->getChromiumProfile());
	dumpProfile(logger, "Berkelium", runtime->forProfile("berkelium"));
}
