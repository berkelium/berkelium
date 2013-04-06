// Copyright (c) 2013 The Berkelium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Berkelium/API/Logger.hpp>
#include <Berkelium/API/Runtime.hpp>

#include <cstring>

namespace Berkelium {

Logger::Logger() {
}

Logger::~Logger() {
}

namespace impl {

/*
class LoggerLineBuffer : public std::stringbuf {
public:
	LoggerLineBuffer() :
		std::stringbuf() {
	}
};

class LoggerStream : public std::stringstream {
public:
	LoggerLine() :
		std::ios(),
		std::ostream(new std::stringbuf()) {
	}

	~LoggerLine() {
		std::cerr << "'" << "test" << "'" << std::endl;
		delete rdbuf();
	}
};
*/

class LoggerImpl : public Logger {
private:
	std::string prefix;

public:
	LoggerImpl(RuntimeRef runtime, const std::string& clazz, const std::string& name) :
		Logger(),
		prefix("\033[0;33mLib  "){
	}

	virtual ~LoggerImpl() {
	}

	virtual std::ostream& debug() {
		return std::cerr << prefix << "\033[1;34mDebug\033[1;33m:\033[0;37m ";
	}

	virtual std::ostream& info() {
		return std::cerr << prefix << "\033[1;32mInfo \033[1;33m:\033[1;37m ";
	}

	virtual std::ostream& warn() {
		return std::cerr << prefix << "\033[1;33mWarn \033[1;33m:\033[1;37m ";
	}

	virtual std::ostream& error() {
		return std::cerr << prefix << "\033[1;31mError\033[1;33m:\033[1;37m ";
	}

	virtual void systemError(const std::string& msg) {
		error() << msg << ": " << strerror(errno) << std::endl;
	}

	virtual void systemError(const std::string& msg, const std::string& arg) {
		error() << msg << ": " << strerror(errno) << " " << arg << std::endl;
	}

	virtual void debug(const std::string& message) {
	}

	virtual void info(const std::string& message) {
	}

	virtual void warn(const std::string& message) {
	}

	virtual void error(const std::string& message) {
	}
};
/*

void setLoggerPrefix(const std::string& prefix) {
	Berkelium::Log::prefix = "\033[0;33m" + prefix + " ";
}


void setLoggerPrefix(RuntimeRef runtime, const std::string& name) {
}

*/

LoggerRef newLogger(RuntimeRef runtime, const std::string& clazz, const std::string& name) {
	LoggerImpl* impl = new LoggerImpl(runtime, clazz, name);
	LoggerRef ret(impl);
	return ret;
}

} // namespace impl

namespace Util {

LoggerRef createRootLogger(RuntimeRef runtime, std::ostream& out) {
	LoggerRef ret(runtime->getLogger("Root", "Logger"));

	return ret;
}

} // namespace Util

} // namespace Berkelium
