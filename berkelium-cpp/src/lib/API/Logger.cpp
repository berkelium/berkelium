// Copyright (c) 2013 The Berkelium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Berkelium/API/Logger.hpp>
#include <Berkelium/API/Runtime.hpp>
#include <Berkelium/API/LogDelegate.hpp>
#include <Berkelium/Impl/Impl.hpp>

#include <cstring>
#include <sstream>

namespace Berkelium {

Logger::Logger() {
}

Logger::~Logger() {
}

namespace impl {

LogSource logSource(Lib);

class LoggerStreamBuffer : public std::stringbuf {
private:
	LogDelegateRef target;
	RuntimeRef runtime;
	LogType type;
	std::string clazz;
	std::string name;

public:
	LoggerStreamBuffer() :
		std::stringbuf(),
		target(),
		runtime(),
		type(),
		clazz(),
		name() {
	}

	virtual int sync() {
		std::string msg(str());
		str(std::string());
		if(msg.length() > 0) {
			msg.erase(msg.length() - 1);
		}
		target->log(runtime, logSource, type, clazz, name, msg);
		return 0;
	}

	void setMode(LogDelegateRef target, RuntimeRef runtime, const LogType type, const std::string& clazz, const std::string& name) {
		this->target = target;
		this->runtime = runtime;
		this->type = type;
		this->clazz = clazz;
		this->name = name;
	}
};

class LoggerStream : public std::ostream {
private:
	LoggerStreamBuffer* buf;

public:
	LoggerStream() :
		std::ostream(new LoggerStreamBuffer()),
		buf((LoggerStreamBuffer*)rdbuf()) {
	}

	~LoggerStream() {
		delete rdbuf();
	}

	void setMode(LogDelegateRef target, RuntimeRef runtime, const LogType type, const std::string& clazz, const std::string& name) {
		buf->setMode(target, runtime, type, clazz, name);
	}
};

LoggerStream logStream;

class LoggerImpl : public Logger {
private:
	LogDelegateRef target;
	RuntimeRef runtime;
	const std::string clazz;
	const std::string name;
	std::string prefix;

public:
	LoggerImpl(LogDelegateRef target, RuntimeRef runtime, const std::string& clazz, const std::string& name) :
		Logger(),
		target(target),
		runtime(runtime),
		clazz(clazz),
		name(name),
		prefix(){
	}

	virtual ~LoggerImpl() {
	}

	virtual std::ostream& debug() {
		logStream.setMode(target, runtime, Debug, clazz, name);
		return logStream;
	}

	virtual std::ostream& info() {
		logStream.setMode(target, runtime, Info, clazz, name);
		return logStream;
	}

	virtual std::ostream& warn() {
		logStream.setMode(target, runtime, Warn, clazz, name);
		return logStream;
	}

	virtual std::ostream& error() {
		logStream.setMode(target, runtime, Error, clazz, name);
		return logStream;
	}

#ifdef _MSC_VER
	inline std::string strerror(int code) {
		char buffer[256] = "";
		strerror_s(buffer, sizeof(buffer), code);
		return buffer;
	}
#endif

	virtual void systemError(const std::string& msg) {
		error() << msg << ": " << strerror(errno) << std::endl;
	}

	virtual void systemError(const std::string& msg, const std::string& arg) {
		error() << msg << ": " << strerror(errno) << " " << arg << std::endl;
	}

	virtual void debug(const std::string& message) {
		target->log(runtime, logSource, LogType::Debug, clazz, name, message);
	}

	virtual void info(const std::string& message) {
		target->log(runtime, logSource, LogType::Info, clazz, name, message);
	}

	virtual void warn(const std::string& message) {
		target->log(runtime, logSource, LogType::Warn, clazz, name, message);
	}

	virtual void error(const std::string& message) {
		target->log(runtime, logSource, LogType::Error, clazz, name, message);
	}
};

void enableBerkeliumHostMode() {
	logSource = Host;
}

LoggerRef newLogger(LogDelegateRef target, RuntimeRef runtime, const std::string& clazz, const std::string& name) {
	LoggerImpl* impl = new LoggerImpl(target, runtime, clazz, name);
	LoggerRef ret(impl);
	return ret;
}

} // namespace impl

namespace Util {

LoggerRef createRootLogger(RuntimeRef runtime) {
	return impl::newLogger(impl::newLogDelegate(), runtime, "Root", "Logger");
}

LoggerRef createRootLogger(LogDelegateRef target, RuntimeRef runtime) {
	return impl::newLogger(target, runtime, "Root", "Logger");
}

} // namespace Util

} // namespace Berkelium
