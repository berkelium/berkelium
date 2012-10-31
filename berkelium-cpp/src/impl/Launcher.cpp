// Copyright (c) 2012 The Berkelium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "berkelium/BerkeliumFactory.hpp"
#include "berkelium/Profile.hpp"
#include "berkelium/HostExecutable.hpp"
#include "berkelium/Impl.hpp"

#include "Process.hpp"

#include <iostream>

namespace Berkelium {

#ifdef WIN32
#define STR_START "\"" +
#define STR_END + "\""
#else
#define STR_START
#define STR_END
#endif

InstanceRef BerkeliumFactory::open(HostExecutableRef executable, ProfileRef profile) {
	InstanceRef ret;

	if(profile->isInUse()) {
		return ret;
	}

	std::vector<std::string> args;
	args.push_back(STR_START executable->getPath() STR_END);
	args.push_back("--no-first-run");
	//args.push_back("--crash-test");
	args.push_back("--user-data-dir=" + STR_START profile->getProfilePath() STR_END);
	args.push_back("about:blank");

	impl::ProcessRef process = impl::Process::create();
	if(!process->start(args)) {
		return ret;
	}
	std::cerr << "waiting..." << std::endl;
	while(!profile->isInUse()) {
		impl::sleep(100);
	}
	std::cerr << "running!" << std::endl;
	return ret;
}

} // namespace Berkelium
