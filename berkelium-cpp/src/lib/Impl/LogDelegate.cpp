// Copyright (c) 2013 The Berkelium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Berkelium/API/LogDelegate.hpp>

namespace Berkelium {

namespace impl {

/*
#include <cstring>
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

class LogDelegateImpl : public LogDelegate {
public:
	LogDelegateImpl() :
		LogDelegate() {
	}

	virtual ~LogDelegateImpl() {
	}

/*
	virtual std::ostream debug() {
		return LoggerLine();
		//return std::cerr << prefix << "\033[1;34mDebug\033[1;33m:\033[0;37m ";
	}

	virtual std::ostream info() {
		return LoggerLine();
//		return std::cerr << prefix << "\033[1;32mInfo \033[1;33m:\033[1;37m ";
	}

	virtual std::ostream warn() {
		return LoggerLine();
//		return std::cerr << prefix << "\033[1;33mWarn \033[1;33m:\033[1;37m ";
	}

	virtual std::ostream error() {
		return LoggerLine();
//		return std::cerr << prefix << "\033[1;31mError\033[1;33m:\033[1;37m ";
	}

	virtual void systemError(const std::string& error) {
//		Log::error() << error << ": " << strerror(errno) << std::endl;
	}

	virtual void systemError(const std::string& error, const std::string& arg) {
//		Log::error() << error << ": " << strerror(errno) << " " << arg << std::endl;
	}

	// Allows the client application to handle berkelium host console messages.
	virtual void log(LogType type, const std::string& message) = 0;
	virtual void debug(const std::string& message) = 0;
	virtual void info(const std::string& message) = 0;
	virtual void warn(const std::string& message) = 0;
	virtual void error(const std::string& message) = 0;
*/
};
/*
std::string prefix = "\033[0;33mLib  ";

void setLoggerPrefix(const std::string& prefix) {
	Berkelium::Log::prefix = "\033[0;33m" + prefix + " ";
}


void setLoggerPrefix(RuntimeRef runtime, const std::string& name) {
}

LogDelegateRef newLogger(RuntimeRef runtime, const std::string& name) {
	return WindowImpl::newWindow(instance, channel, incognito);
}
*/

} // namespace impl

} // namespace Berkelium
