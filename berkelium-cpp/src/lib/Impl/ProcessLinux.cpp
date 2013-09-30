// Copyright (c) 2012 The Berkelium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifdef LINUX

#include <Berkelium/API/Logger.hpp>
#include <Berkelium/Impl/Impl.hpp>
#include <Berkelium/Impl/Process.hpp>
#include <Berkelium/IPC/Channel.hpp>
#include <Berkelium/IPC/ChannelGroup.hpp>

#include <sys/wait.h>
#include <unistd.h>

namespace Berkelium {

namespace impl {

int exec(const std::vector<std::string>& args) {
	const size_t size = args.size();
	std::vector<char*> tmp(size + 1);
	for(size_t i = 0; i < size; i++) {
		tmp[i] = (char*)args[i].c_str();
	}
	tmp[size] = NULL;
    return execvp(args[0].c_str(), &tmp[0]);
}

#define BUF_SIZE 1024

class ConsoleWriter : public Berkelium::Ipc::ChannelCallback
{
public:
	virtual ~ConsoleWriter() {
		//bk_error("~ConsoleWriter");
	}

	virtual void onDataReady(Ipc::ChannelRef channel) {
		//bk_error("ConsoleWriter: onDataReady!");
		int fd = getPipeFd(channel, true);
		char buf[BUF_SIZE];
		int r = ::read(fd, &buf, BUF_SIZE);
		if(r == 0) {
			return;
		} else if(r == -1) {
			bk_error("ConsoleWriter: read error!");
			return;
		}
		write(1, buf, r);
	}
};

class ProcessLinuxImpl : public Process {
private:
	std::shared_ptr<ConsoleWriter> writer;
	pid_t pid;
	int exit;

public:
	ProcessLinuxImpl(Ipc::ChannelGroupRef group, LoggerRef logger, const std::string& dir) :
		Process(group, logger, dir),
		writer(new ConsoleWriter()),
		pid(-1),
		exit(-1) {
		group->registerCallback(getIpcOut(), writer, true);
		group->registerCallback(getIpcErr(), writer, true);
	}

	virtual ~ProcessLinuxImpl() {
		if(pid == -1) return;
		logger->debug() << "waiting for pid " << pid << "..." << std::endl;
		wait(0);
	}

	bool wait(int options) {
		int status;
		int wp = waitpid(pid, &status, options);
		if (wp != -1) {
			if(wp == 0 && options == WNOHANG) {
				return true;
			}
			logger->debug() << "Child exited with status " << status << std::endl;
			exit = status;
			pid = -1;
			logger->info() << "berkelium host process terminated!" << std::endl;
			return false;
		} else {
			logger->systemError("waitpid");
		}
		return true;
	}

	virtual bool isRunning() {
		if(exit != -1) {
			return false;
		}
		return wait(WNOHANG);
	}


	virtual const bool start(const std::vector<std::string>& args) {
		pid = fork();
		switch (pid) {
		case -1: {
			logger->systemError("fork");
			return false;
		}
		case 0: {
			dup2(getPipeFd(getIpcOut(), true), 1);
			dup2(getPipeFd(getIpcErr(), false), 2);
			int ret = exec(args);
			logger->systemError(("launch " + args[0]).c_str());
			::exit(ret);
			break;
		}
		default: {
			logger->debug() << "started berkelium host process with pid " << pid << "!" << std::endl;
		}
		}
		return true;
	}
};

ProcessRef Process::create(Ipc::ChannelGroupRef group, LoggerRef logger, const std::string& dir) {
	return ProcessRef(new ProcessLinuxImpl(group, logger, dir));
}

Process::~Process() {
}

} // namespace impl

} // namespace Berkelium

#endif // LINUX
