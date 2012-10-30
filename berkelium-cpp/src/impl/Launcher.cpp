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

InstanceRef BerkeliumFactory::open(HostExecutableRef executable, ProfileRef profile) {
	InstanceRef ret;

	if(profile->isInUse()) {
		return ret;
	}

	std::string cmd = "\"" + executable->getPath();
	cmd += "\" --no-first-run --user-data-dir=\"";
	// --crash-test
	cmd += profile->getProfilePath();
	cmd += "\" about:blank";

	impl::ProcessRef process = impl::Process::create();
	if(!process->start(cmd)) {
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
