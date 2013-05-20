// Copyright (c) 2012 The Berkelium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifdef OS_WINDOWS

#include <Berkelium/API/BerkeliumFactory.hpp>
#include <Berkelium/API/Logger.hpp>
#include <Berkelium/IPC/Message.hpp>
#include <Berkelium/IPC/Pipe.hpp>
#include <Berkelium/Impl/Filesystem.hpp>

using Berkelium::impl::Filesystem;

namespace Berkelium {

namespace Ipc {

namespace impl {

class PipeWindowsImpl : public Pipe {
private:
	LoggerRef logger;
	const std::string name;

public:
	PipeWindowsImpl(LoggerRef logger, const std::string& name) :
		Pipe(),
		logger(logger),
		name(name) {

		Filesystem::createDirectoriesFor(name);
	}

	virtual ~PipeWindowsImpl() {
		Filesystem::removeFile(name);
	}

	virtual bool isEmpty() {
		return true;
	}

	virtual void send(MessageRef msg) {
	}

	virtual void recv(MessageRef msg) {
	}

	void recv(char* to, size_t size) {
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
	return PipeRef(new impl::PipeWindowsImpl(logger, name));
}

} // namespace Ipc

} // namespace Berkelium

#endif // OS_WINDOWS
