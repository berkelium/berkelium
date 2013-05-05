// Copyright (c) 2012 The Berkelium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifdef LINUX

#include <Berkelium/API/BerkeliumFactory.hpp>
#include <Berkelium/API/Logger.hpp>
#include <Berkelium/IPC/Message.hpp>
#include <Berkelium/IPC/Pipe.hpp>
#include <Berkelium/Impl/Filesystem.hpp>

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

using Berkelium::impl::Filesystem;

namespace Berkelium {

namespace Ipc {

namespace impl {

class PipeLinuxImpl : public Pipe {
private:
	LoggerRef logger;
	const std::string name;
	int fd;
	fd_set fds;

public:
	PipeLinuxImpl(LoggerRef logger, const std::string& name) :
		Pipe(),
		logger(logger),
		name(name) {
		fd = -1;

		Filesystem::createDirectoriesFor(name);

		const char* p = name.c_str();
		if(::access(p, F_OK) != 0 && ::mkfifo(p, 0700) != 0) {
			logger->systemError("mkfifo", name);
			return;
		}
		fd = ::open(p, O_RDWR);
		if(fd == -1) {
			logger->systemError("open", name);
			return;
		}
	}

	virtual ~PipeLinuxImpl() {
		if(fd != -1) {
			close(fd);
			fd = -1;
		}
		Filesystem::removeFile(name);
	}

	virtual bool isEmpty() {
		static struct timeval tv;
		tv.tv_sec = 0;
		tv.tv_usec = 0;
		FD_ZERO(&fds);
		FD_SET(fd, &fds);
		return select(fd + 1, &fds, NULL, NULL, &tv) != 1;
	}

	virtual void send(MessageRef msg) {
		if(fd == -1) return;
		int32_t size = msg->length();
		//fprintf(stderr, "send: 4 bytes '%d'!\n", size);
		::write(fd, &size, 4);
		//fprintf(stderr, "send: data...\n");
		::write(fd, msg->data(), size);
		//fprintf(stderr, "send: done!\n");
		msg->reset();
	}

	virtual void recv(MessageRef msg) {
		int32_t size = 0;
		recv((char*)&size, 4);
		//fprintf(stderr, "recv: %d bytes!\n", size);
		msg->setup(size);
		recv((char*)msg->data(), size);
	}

	void recv(char* to, size_t size) {
		size_t off = 0;
		while(off < size) {
			//fprintf(stderr, "reading %ld of %ld...\n", off, size);
			ssize_t ret = ::read(fd, &to[off], size);
			//fprintf(stderr, "read: %ld bytes of %ld!\n", ret, size);
			if(ret == -1) break;
			off += ret;
		}
	}

	virtual const std::string getPath() {
		return name;
	}
};

} // namespace impl

Pipe::Pipe() {
}

Pipe::~Pipe() {
}

PipeRef Pipe::getPipe(LoggerRef logger, const std::string& name) {
	return PipeRef(new impl::PipeLinuxImpl(logger, name));
}

} // namespace Ipc

} // namespace Berkelium

#endif // LINUX
