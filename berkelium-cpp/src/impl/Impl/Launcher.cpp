// Copyright (c) 2012 The Berkelium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Berkelium/API/BerkeliumFactory.hpp>
#include <Berkelium/API/Profile.hpp>
#include <Berkelium/API/HostExecutable.hpp>
#include <Berkelium/IPC/IpcMessage.hpp>
#include <Berkelium/Impl/Impl.hpp>
#include <Berkelium/Impl/Process.hpp>

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
	if(profile->isInUse()) {
		return InstanceRef();
	}

	impl::ProcessRef process = impl::Process::create(profile->getProfilePath());
	impl::IpcRef ipc = process->getIpc();

	std::vector<std::string> args;
	args.push_back(enclose(executable->getPath()));
	args.push_back("--berkelium=" + ipc->getName());
	args.push_back("--no-first-run");
	//args.push_back("--crash-test");
	args.push_back("--user-data-dir=" + enclose(profile->getProfilePath()));
	args.push_back("about:blank");

	if(!process->start(args)) {
		return InstanceRef();
	}

	std::cerr << "awaiting berkelium host process ipc startup message!" << std::endl;
	impl::IpcMessageRef msg(impl::IpcMessage::create());
	ipc->recv(msg);
	if(msg->get_str().compare("berkelium") != 0) {
		std::cerr << "ipc bad magic!" << std::endl;
		return InstanceRef();
	}
	std::cerr << "waiting for profile..." << std::endl;
	while(!profile->isInUse()) {
		impl::sleep(100);
	}
	/*
	if(!profile->isInUse()) {
		std::cerr << "profile not in use!" << std::endl;
		return ret;
	}
	*/
	std::cerr << "berkelium host process is up and running!" << std::endl;

	return impl::newInstance(executable, profile, ipc, process);
}

} // namespace Berkelium
