// Copyright (c) 2012 The Berkelium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifdef LINUX

#include <Berkelium/Impl/Impl.hpp>
#include <Berkelium/Impl/Process.hpp>
#include <Berkelium/Impl/Logger.hpp>

#include <sys/wait.h>

namespace Berkelium {

namespace impl {

int exec(const std::vector<std::string>& args) {
	const size_t size = args.size();
	std::vector<char*> tmp(size + 1);
	//printf("'%s'\n", args[0].c_str());
	for(size_t i = 0; i < size; i++) {
		tmp[i] = (char*)args[i].c_str();
		//printf("arg[%lu]='%s'\n", i, tmp[i]);
	}
	tmp[size] = NULL;
    return execvp(args[0].c_str(), &tmp[0]);
}

class ProcessLinuxImpl : public Process {
private:
	pid_t pid;
	int exit;

public:
	ProcessLinuxImpl(const std::string& dir) : Process(dir),
		pid(-1),
		exit(-1) {
	}

	virtual ~ProcessLinuxImpl() {
		if(pid == -1) return;
		Log::debug() << "waiting for pid " << pid << "..." << std::endl;
		wait(0);
	}

	bool wait(int options) {
		int status;
		int wp = waitpid(pid, &status, options);
		if (wp != -1) {
			if(wp == 0 && options == WNOHANG) {
				return true;
			}
			Log::debug() << "Child exited with status " << status << std::endl;
			exit = status;
			pid = -1;
			Log::info() << "berkelium host process terminated!" << std::endl;
			return false;
		} else {
			perror("waitpid");
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
			perror("fork");
			return false;
		}
		case 0: {
			int ret = exec(args);
			perror(("launch " + args[0]).c_str());
			::exit(ret);
			break;
		}
		default: {
			Log::debug() << "started berkelium host process with pid " << pid << "!" << std::endl;
		}
		}
		return true;
	}
};

ProcessRef Process::create(const std::string& dir) {
	return ProcessRef(new ProcessLinuxImpl(dir));
}

Process::~Process() {
}

} // namespace impl

} // namespace Berkelium

#endif // LINUX
