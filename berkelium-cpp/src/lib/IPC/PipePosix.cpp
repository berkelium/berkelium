// Copyright (c) 2012 The Berkelium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifdef LINUX

#include <Berkelium/API/BerkeliumFactory.hpp>
#include <Berkelium/API/Logger.hpp>
#include <Berkelium/IPC/Message.hpp>
#include <Berkelium/IPC/PipePosix.hpp>
#include <Berkelium/Impl/Filesystem.hpp>
#include <Berkelium/Impl/Impl.hpp>

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sstream>

#ifdef BERKELIUM_TRACE_IO_DATA
#define trace 1
#else
#define trace 0
#endif

using Berkelium::impl::Filesystem;

namespace Berkelium {

namespace Ipc {

namespace impl {

class PipePosixImpl : public PipePosix {
private:
	LoggerRef logger;
	const std::string name;
	const std::string dir;
	const std::string full;
	const std::string alias;
	bool read;
	int fd;
	fd_set fds;

public:
	PipePosixImpl(bool create, bool read, LoggerRef logger, const std::string& dir, const std::string& name, const std::string& alias) :
		PipePosix(),
		logger(logger),
		name(name),
		dir(dir),
		full(Filesystem::append(dir, name)),
		alias(alias),
		read(read),
		fd(-1),
		fds() {
		TRACE_OBJECT_NEW("PipePosixImpl");

		Filesystem::createDirectories(dir);

		const char* p = full.c_str();
		if(::access(p, F_OK) != 0) {
			if(!create) {
				logger->systemError("fifo not found", full);
				return;
			}
			if(::mkfifo(p, 0700) != 0) {
				logger->systemError("mkfifo", full);
				return;
			}
		} else if(create) {
			logger->systemError("fifo already exists", full);
			return;
		}
		fd = ::open(p, O_RDWR);
		if(fd == -1) {
			logger->systemError("open", full);
			return;
		}

		//fprintf(stderr, "new Link %s %d %s\n", name.c_str(), fd, alias.c_str());
	}

	virtual ~PipePosixImpl() {
		TRACE_OBJECT_DELETE("PipePosixImpl");
		if(fd != -1) {
			close(fd);
			fd = -1;
		}
		Filesystem::removeFile(full);
	}

	virtual bool isOk() {
		return fd != -1;
	}

	virtual bool isEmpty() {
		if(fd == -1) {
			logger->systemError("bad pipe: isEmpty", full);
			return true;
		}
		static struct timeval tv;
		tv.tv_sec = 0;
		tv.tv_usec = 0;
		FD_ZERO(&fds);
		FD_SET(fd, &fds);
		return select(fd + 1, &fds, NULL, NULL, &tv) != 1;
	}

	inline void dump(const char* msg, int fd, int32_t ch, char* data, size_t size) {
		std::stringstream str;
		str << Berkelium::impl::getBerkeliumHostMode();
		str << ' ';
		str << msg;
		str << ": ";
		str << fd;
		str << ' ';
		str << name;
		str << ' ';
		str << alias;
		str << '(';
		str << ch;
		str << ") ";
		str << std::hex;
		for(size_t i = 0; i < size; i++) {
			str << (int)(data[i] & 0xFF);
			str << ' ';
		}
		str << ' ';
		for(size_t i = 0; i < size; i++) {
			str << (char)(data[i] >= 32 ? data[i] : '.');
		}
		fprintf(stderr, "%s\n", str.str().c_str());
	}

	virtual void send(MessageRef msg) {
		if(fd == -1) {
			logger->systemError("bad pipe: send", full);
			return;
		}
		int32_t size = msg->data_length();
		//fprintf(stderr, "send: 4 bytes '%d'!\n", size);
		::write(fd, &size, 4);
		//fprintf(stderr, "send: data...\n");
		if(trace) {
			dump("send", fd, msg->getChannelId(), (char*)msg->data(), size);
		}
		::write(fd, msg->data(), size);
		//fprintf(stderr, "send: done!\n");
		msg->reset();
	}

	virtual MessageRef recv() {
		MessageRef msg(Message::create(logger));
		if(fd == -1) {
			logger->systemError("bad pipe: recv", full);
			return msg;
		}
		int32_t size = 0;
		recv((char*)&size, 4);
		//fprintf(stderr, "recv: %d bytes!\n", size);
		msg->setup(size - sizeof(int32_t));
		recv((char*)msg->data(), size);
		if(trace) {
			dump("recv", fd, msg->getChannelId(), (char*)msg->data(), size);
		}
		return msg;
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

	virtual const std::string getName() {
		return name;
	}

	virtual const std::string getAlias() {
		return alias;
	}

	int getLinkFd() {
		if(!read) {
			return -1;
		}
		return fd;
	}
};

} // namespace impl

PipePosix::PipePosix() :
	Link() {
}

PipePosix::~PipePosix() {
}

PipePosixRef PipePosix::create(bool create, bool read, LoggerRef logger, const std::string& dir, const std::string& name, const std::string& alias) {
	impl::PipePosixImpl* impl(new impl::PipePosixImpl(create, read, logger, dir, name, alias));
	PipePosixRef ret(impl);
	if(!impl->isOk()) {
		return PipePosixRef();
	}
	return ret;
}

} // namespace Ipc

namespace impl {

int getLinkFd(Ipc::PipePosixRef pipe) {
	if(!pipe) {
		Berkelium::impl::bk_error("getLinkFd: pipe is NULL!");
		return 0;
	}
	return ((Ipc::impl::PipePosixImpl*)pipe.get())->getLinkFd();
}

} // namespace impl

} // namespace Berkelium

#endif // LINUX
