// Copyright (c) 2013 The Berkelium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Berkelium/API/Logger.hpp>
#include <Berkelium/API/Runtime.hpp>

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
	RuntimeRef runtime;
	LogType type;
	std::string clazz;
	std::string name;

public:
	LoggerStreamBuffer() :
		std::stringbuf(),
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
		runtime->log(logSource, type, clazz, name, msg);
		return 0;
	}

	void setMode(RuntimeRef runtime, const LogType type, const std::string& clazz, const std::string& name) {
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

	void setMode(RuntimeRef runtime, const LogType type, const std::string& clazz, const std::string& name) {
		buf->setMode(runtime, type, clazz, name);
	}
};

LoggerStream logStream;

class LoggerImpl : public Logger {
private:
	RuntimeRef runtime;
	const std::string clazz;
	const std::string name;
	std::string prefix;

public:
	LoggerImpl(RuntimeRef runtime, const std::string& clazz, const std::string& name) :
		Logger(),
		runtime(runtime),
		clazz(clazz),
		name(name),
		prefix(){
	}

	virtual ~LoggerImpl() {
	}

	virtual std::ostream& debug() {
		logStream.setMode(runtime, Debug, clazz, name);
		return logStream;
	}

	virtual std::ostream& info() {
		logStream.setMode(runtime, Info, clazz, name);
		return logStream;
	}

	virtual std::ostream& warn() {
		logStream.setMode(runtime, Warn, clazz, name);
		return logStream;
	}

	virtual std::ostream& error() {
		logStream.setMode(runtime, Error, clazz, name);
		return logStream;
	}

	virtual void systemError(const std::string& msg) {
		error() << msg << ": " << strerror(errno) << std::endl;
	}

	virtual void systemError(const std::string& msg, const std::string& arg) {
		error() << msg << ": " << strerror(errno) << " " << arg << std::endl;
	}

	virtual void debug(const std::string& message) {
		runtime->log(logSource, LogType::Debug, clazz, name, message);
	}

	virtual void info(const std::string& message) {
		runtime->log(logSource, LogType::Info, clazz, name, message);
	}

	virtual void warn(const std::string& message) {
		runtime->log(logSource, LogType::Warn, clazz, name, message);
	}

	virtual void error(const std::string& message) {
		runtime->log(logSource, LogType::Error, clazz, name, message);
	}
};

void enableBerkeliumHostMode() {
	logSource = Host;
}

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
