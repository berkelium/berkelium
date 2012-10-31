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

namespace {

inline std::string enclose(const std::string& str) {
#ifdef WIN32
	return "\"" + str + "\"";
#else
	return str;
#endif
}

} // namespace

InstanceRef BerkeliumFactory::open(HostExecutableRef executable, ProfileRef profile) {
	InstanceRef ret;

	if(profile->isInUse()) {
		return ret;
	}

	std::vector<std::string> args;
	args.push_back(enclose(executable->getPath()));
	args.push_back("--no-first-run");
	//args.push_back("--crash-test");
	args.push_back("--user-data-dir=" + enclose(profile->getProfilePath()));
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
