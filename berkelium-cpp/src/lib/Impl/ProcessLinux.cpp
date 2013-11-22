// Copyright (c) 2012 The Berkelium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifdef LINUX

#include <Berkelium/API/Logger.hpp>
#include <Berkelium/API/Runtime.hpp>
#include <Berkelium/Impl/Impl.hpp>
#include <Berkelium/Impl/Process.hpp>
#include <Berkelium/IPC/Link.hpp>
#include <Berkelium/IPC/LinkGroup.hpp>

#include <sys/wait.h>
#include <unistd.h>

#ifdef BERKELIUM_NO_HOST_REDIRECT
#define redirect 0
#else
#define redirect 1
#endif

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

using Berkelium::Ipc::LinkCallback;
using Berkelium::Ipc::LinkCallbackRef;

class ProcessLinuxImpl;
typedef std::shared_ptr<ProcessLinuxImpl> ProcessLinuxImplRef;

class ProcessLinuxImpl : public Process, public LinkCallback {
private:
	Ipc::LinkRef pipeout;
	Ipc::LinkRef pipeerr;
	pid_t pid;
	int exit;

public:
	ProcessLinuxImpl(RuntimeRef runtime, LoggerRef logger, const std::string& dir) :
		Process(runtime, logger, dir),
		pipeout(Ipc::Link::getLink(true, group, logger, dir, name + "1", "process.out")),
		pipeerr(Ipc::Link::getLink(true, group, logger, dir, name + "2", "process.err")),
		pid(-1),
		exit(-1) {
		TRACE_OBJECT_NEW("ProcessLinux");
	}

	void registerCallbacks(ProcessLinuxImplRef self) {
		group->registerCallback(pipeout, self);
		group->registerCallback(pipeerr, self);
	}

	virtual ~ProcessLinuxImpl() {
		TRACE_OBJECT_DELETE("ProcessLinux");
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
			exit = WEXITSTATUS(status);
			logger->debug() << "Child exited with status " << exit << std::endl;
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

	void update(std::string& buffer, Ipc::LinkRef link, ConsoleRedirector& cr) {
		int fd = getLinkFd(link);
		char buf[BUF_SIZE];

		// read everything available into buffer
		while(!link->isEmpty()) {
			int r = ::read(fd, &buf, BUF_SIZE);
			if(r == 0) {
				break;
			} else if(r == -1) {
				bk_error("ConsoleWriter: read error!");
				return;
			}
			buffer += std::string(buf, 0, r);
		}
		cr.update(buffer);
	}

	virtual void onLinkDataReady(Berkelium::Ipc::LinkRef link, uint32_t size, uint8_t* data) {
		if(link == pipeout) {
			bufout += std::string((const char*)data, 0, size);
			out.update(bufout);
		} else if(link == pipeerr) {
			buferr += std::string((const char*)data, 0, size);
			err.update(buferr);
		}
	}

	virtual bool isCrashed() {
		return exit > 0;
	}

	virtual const bool start(const std::vector<std::string>& args) {
		pid = fork();
		switch (pid) {
		case -1: {
			logger->systemError("fork");
			return false;
		}
		case 0: {
			if(redirect) {
				dup2(getLinkFd(pipeout), 1);
				dup2(getLinkFd(pipeerr), 2);
			}
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

ProcessRef Process::create(RuntimeRef runtime, LoggerRef logger, const std::string& dir) {
	ProcessLinuxImpl* impl(new ProcessLinuxImpl(runtime, logger, dir));
	ProcessLinuxImplRef ret(impl);
	if(!impl->channels) {
		fprintf(stderr, "create process failed\n");
		return ProcessRef();
	}
	impl->registerCallbacks(ret);
	return ret;
}

} // namespace impl

} // namespace Berkelium

#endif // LINUX
