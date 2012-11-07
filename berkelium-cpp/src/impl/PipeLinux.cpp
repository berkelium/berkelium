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
		//fprintf(stderr, "send: 4 bytes '%d'!\n", size);
		::write(fd, &size, 4);
		//fprintf(stderr, "send: data...\n");
		::write(fd, msg->data(), size);
		//fprintf(stderr, "send: done!\n");
		msg->reset();
	}

	virtual void recv(IpcMessageRef msg) {
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
		return name.string();
	}
};

PipeRef Pipe::getPipe(const std::string& name) {
	return PipeRef(new impl::PipeLinuxImpl(name));
}

} // namespace impl

} // namespace Berkelium

#endif // LINUX
