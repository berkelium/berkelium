// Copyright (c) 2012 The Berkelium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifdef LINUX

#include "berkelium/BerkeliumFactory.hpp"
#include "berkelium/IpcMessage.hpp"
#include "berkelium/Pipe.hpp"

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <boost/filesystem.hpp>

using boost::filesystem::path;

namespace Berkelium {

namespace impl {

Pipe::Pipe() {
}

Pipe::~Pipe() {
}

class PipeLinuxImpl : public Pipe {
private:
	const path name;
	int fd;
	fd_set fds;

public:
	PipeLinuxImpl(const std::string& name) :
		name(name) {
		fd = -1;

		const char* p = name.c_str();
		if(::access(p, F_OK) != 0 && ::mkfifo(p, 0700) != 0) {
			perror("mkfifo");
			return;
		}
		fd = ::open(p, O_RDWR);
		if(fd == -1) {
			perror("open");
			return;
		}
	}

	virtual ~PipeLinuxImpl() {
		if(fd != -1) {
			close(fd);
			fd = -1;
		}
		boost::filesystem::remove(name);
	}

	virtual bool isEmpty() {
		static struct timeval tv;
		tv.tv_sec = 0;
		tv.tv_usec = 0;
		FD_ZERO(&fds);
		FD_SET(fd, &fds);
		return select(fd + 1, &fds, NULL, NULL, &tv) != 1;
	}

	virtual void send(IpcMessageRef msg) {
		if(fd == -1) return;
		int32_t size = msg->length();
		::write(fd, &size, 4);
		::write(fd, msg->data(), size);
		msg->reset();
	}

	virtual void recv(IpcMessageRef msg) {
		int32_t size = 0;
		::read(fd, &size, 4);
		msg->setup(size);
		::read(fd, msg->data(), size);
	}

	virtual const std::string getPath() {
		return name.string();
	}
};

PipeRef Pipe::getPipe(const std::string& name) {
	return PipeRef(new impl::PipeLinuxImpl(name));
}

} // namespace impl

} // namespace Berkelium

#endif // LINUX
