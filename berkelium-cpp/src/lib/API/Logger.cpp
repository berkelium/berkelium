// Copyright (c) 2013 The Berkelium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Berkelium/API/Logger.hpp>
#include <Berkelium/API/Runtime.hpp>
#include <Berkelium/API/LogDelegate.hpp>
#include <Berkelium/Impl/Impl.hpp>
#include <Berkelium/Impl/Manager.hpp>

#include <cstring>
#include <sstream>

namespace Berkelium {

Logger::Logger() {
	TRACE_OBJECT_NEW("Logger");
}

Logger::~Logger() {
	TRACE_OBJECT_DELETE("Logger");
}

namespace impl {

LogSource logSource(Lib);

class LoggerStreamBuffer : public std::stringbuf {
private:
	LogDelegateRef target;
	RuntimeWRef runtime;
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
		TRACE_OBJECT_NEW("LoggerStreamBuffer");
	}

	~LoggerStreamBuffer() {
		TRACE_OBJECT_DELETE("LoggerStreamBuffer");
	}

	virtual int sync() {
		RuntimeRef rt(runtime.lock());

		std::string msg(str());
		str(std::string());
		if(msg.length() > 0) {
			msg.erase(msg.length() - 1);
		}
		if(rt) {
			target->log(rt, logSource, type, clazz, name, msg);
		} else {
			bk_error("Warning: runtime lost: can not log '%s' '%s' '%s'!", clazz.c_str(), name.c_str(), msg.c_str());
		}
		return 0;
	}

	void setMode(LogDelegateRef target, RuntimeWRef runtime, const LogType type, const std::string& clazz, const std::string& name) {
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
	LoggerStream(LoggerStreamBuffer* buf) :
		std::ostream(buf),
		buf(buf) {
		TRACE_OBJECT_NEW("LoggerStream");
	}

	~LoggerStream() {
		TRACE_OBJECT_DELETE("LoggerStream");
	}

	void setMode(LogDelegateRef target, RuntimeWRef runtime, const LogType type, const std::string& clazz, const std::string& name) {
		buf->setMode(target, runtime, type, clazz, name);
	}
};

class LoggerImpl : public Logger {
private:
	LogDelegateRef target;
	RuntimeWRef runtime;
	const ManagerRef manager;
	LoggerStreamBuffer logBuffer;
	LoggerStream logStream;
	const std::string clazz;
	const std::string name;
	std::string prefix;

public:
	LoggerImpl(LogDelegateRef target, RuntimeRef runtime, const std::string& clazz, const std::string& name) :
		Logger(),
		target(target),
		runtime(runtime),
		manager(Berkelium::impl::getManager(runtime)),
		logBuffer(),
		logStream(&logBuffer),
		clazz(clazz),
		name(name),
		prefix(){
		TRACE_OBJECT_NEW("LoggerImpl");

	}

	virtual ~LoggerImpl() {
		TRACE_OBJECT_DELETE("LoggerImpl");
		if(manager) {
			manager->unregisterLogger();
		}
	}

	ManagerRef getManager() {					\
		return manager;							\
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
		target->log(runtime.lock(), logSource, LogType::Debug, clazz, name, message);
	}

	virtual void info(const std::string& message) {
		target->log(runtime.lock(), logSource, LogType::Info, clazz, name, message);
	}

	virtual void warn(const std::string& message) {
		target->log(runtime.lock(), logSource, LogType::Warn, clazz, name, message);
	}

	virtual void error(const std::string& message) {
		target->log(runtime.lock(), logSource, LogType::Error, clazz, name, message);
	}
};

void enableBerkeliumHostMode() {
	logSource = Host;
}

bool isBerkeliumHostMode() {
	return logSource == Host;
}

const char* getBerkeliumHostMode()
{
	return isBerkeliumHostMode() ? "Host" : "Lib";
}

ManagerRef getManager(Logger* logger)
{
	if(!logger) {
		bk_error("getManager(Logger* = null)");
		return ManagerRef();
	}
	LoggerImpl* impl = (LoggerImpl*)logger;
	return impl->getManager();
}

LoggerRef newLogger(LogDelegateRef target, RuntimeRef runtime, const std::string& clazz, const std::string& name) {
	LoggerImpl* impl = new LoggerImpl(target, runtime, clazz, name);
	LoggerRef ret(impl);
	ManagerRef manager(impl->getManager());
	if(manager) {
		manager->registerLogger(ret);
	}
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
