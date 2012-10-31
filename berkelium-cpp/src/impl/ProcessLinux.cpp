// Copyright (c) 2012 The Berkelium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifdef LINUX

#include "berkelium/Impl.hpp"
#include "Process.hpp"

#include <sys/wait.h>
#include <iostream>

namespace Berkelium {

namespace impl {

int exec(const std::vector<std::string>& args) {
	const size_t size = args.size();
	std::vector<char*> tmp(size);
	printf("'%s'\n", args[0].c_str());
	for(size_t i = 0; i < size; i++) {
		tmp[i] = (char*)args[i].c_str();
		printf("arg[%lu]='%s'\n", i, tmp[i]);
	}
    return execvp(args[0].c_str(), &tmp[0]);
}

class ProcessImpl : public Process {
private:
	pid_t pid;

public:
	ProcessImpl() :
		pid(-1) {
	}

	virtual ~ProcessImpl() {
		if(pid == -1) return;
		int status;
		if (waitpid(pid, &status, 0) != -1) {
			printf("Child exited with status %i\n", status);
		} else {
			perror("waitpid");
		}
	}

	virtual const bool start(const std::vector<std::string>& args) {
		pid_t pid = fork();
		switch (pid) {
		case -1:
			perror("fork");
			break;
		case 0:
			exec(args);
			perror(("launch " + args[0]).c_str());
			break;
		default:
			printf("Child id: %i\n", pid);
		}
		return true;
	}
};

ProcessRef Process::create() {
	return ProcessRef(new ProcessImpl());
}

Process::~Process() {
}

} // namespace impl

} // namespace Berkelium

#endif // LINUX
