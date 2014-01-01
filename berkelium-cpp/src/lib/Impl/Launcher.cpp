// Copyright (c) 2012 The Berkelium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Berkelium/API/BerkeliumFactory.hpp>
#include <Berkelium/API/Profile.hpp>
#include <Berkelium/API/HostExecutable.hpp>
#include <Berkelium/API/Util.hpp>
#include <Berkelium/API/Runtime.hpp>
#include <Berkelium/API/Logger.hpp>
#include <Berkelium/IPC/Message.hpp>
#include <Berkelium/IPC/ChannelGroup.hpp>
#include <Berkelium/Impl/Impl.hpp>

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

	LoggerRef logger(runtime->getLogger("Process", Util::randomId()));
	impl::ProcessRef process = impl::Process::create(runtime, logger, profile->getProfilePath());
	if(!process) {
		fprintf(stderr, "process failed\n");
		return InstanceRef();
	}
	Ipc::ChannelRef ipc = process->getIpcChannel();
	if(!ipc) {
		fprintf(stderr, "getIpcChannel() failed\n");
		return InstanceRef();
	}

	std::vector<std::string> args;
	args.push_back(enclose(executable->getPath()));
	args.push_back("--berkelium=" + process->getChannelGroup()->getName());
	args.push_back("--no-sandbox"); // TODO check why this is needed...
	args.push_back("--no-first-run");
	//args.push_back("--crash-test");
	args.push_back("--user-data-dir=" + enclose(profile->getProfilePath()));
	args.push_back("about:blank");

	if(!process->start(args)) {
		return InstanceRef();
	}

	logger->debug() << "awaiting berkelium host process ipc startup message!" << std::endl;
	Ipc::MessageRef msg;
	while(true) {
		msg = ipc->recv();
		if(msg) {
			break;
		}
		if(!process->isRunning()) {
			logger->error() << "berkelium host startup failed!" << std::endl;
			return InstanceRef();
		}
	}
	if(msg->get_str().compare("berkelium") != 0) {
		logger->error() << "ipc bad magic!" << std::endl;
		return InstanceRef();
	}
	Ipc::ChannelRef ping(ipc->getGroup()->getChannel(msg->get_32(), "ping"));
	msg->reset();
	ipc->send(msg);
	logger->debug() << "waiting for profile..." << std::endl;
	while(!profile->isInUse()) {
		runtime->update(100);
		if(!process->isRunning()) {
			logger->error() << "berkelium host startup failed!" << std::endl;
			return InstanceRef();
		}
	}
	logger->info() << "berkelium host process is up and running!" << std::endl;

	return impl::newInstance(executable, profile, ipc, process, ping);
}

} // namespace impl

} // namespace Berkelium
