// Copyright (c) 2012 The Berkelium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Berkelium/API/BerkeliumFactory.hpp>
#include <Berkelium/API/Profile.hpp>
#include <Berkelium/API/HostExecutable.hpp>
#include <Berkelium/API/Util.hpp>
#include <Berkelium/IPC/Message.hpp>
#include <Berkelium/Impl/Impl.hpp>
#include <Berkelium/Impl/Logger.hpp>

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

namespace impl {

InstanceRef newInstance(RuntimeRef runtime, HostExecutableRef executable, ProfileRef profile) {
	if(profile->isInUse()) {
		return InstanceRef();
	}

	impl::ProcessRef process = impl::Process::create(profile->getProfilePath());
	Ipc::ChannelRef ipc = process->getIpcChannel();

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

	Log::debug() << "awaiting berkelium host process ipc startup message!" << std::endl;
	Ipc::MessageRef msg(ipc->getMessage());
	while(ipc->isEmpty()) {
		if(!process->isRunning()) {
			Log::error() << "berkelium host startup failed!" << std::endl;
			return InstanceRef();
		}
		Util::sleep(100);
	}
	ipc->recv(msg);
	if(msg->get_str().compare("berkelium") != 0) {
		Log::error() << "ipc bad magic!" << std::endl;
		return InstanceRef();
	}
	Log::debug() << "waiting for profile..." << std::endl;
	while(!profile->isInUse()) {
		Util::sleep(100);
	}
	Log::info() << "berkelium host process is up and running!" << std::endl;

	return impl::newInstance(executable, profile, ipc, process);
}

} // namespace impl

} // namespace Berkelium
