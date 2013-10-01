// Copyright (c) 2013 The Berkelium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Berkelium/API/LogDelegate.hpp>

#include <sstream>
#if OS_WINDOWS
#include <windows.h>
#endif

namespace Berkelium {

LogDelegate::LogDelegate() {
}

LogDelegate::~LogDelegate() {
}

namespace impl {

#if !defined(OS_WINDOWS)
const char* COLOR_SRC("\033[0;33m");
const char* COLOR_SEP("\033[1;33m");
const char* COLOR_0("\033[0;37m");
const char* COLOR_1("\033[1;37m");
const char* DEBUG_MSG ("\033[1;34mDebug");
const char* INFO_MSG  ("\033[1;32mInfo ");
const char* WARN_MSG  ("\033[1;33mWarn ");
const char* ERROR_MSG ("\033[1;31mError");
const char* STDOUT_MSG("\033[1;32mOut  ");
const char* STDERR_MSG("\033[1;31mErr  ");
#endif

inline std::string pad(std::string n, int s, char p) {
	int s2 = n.size();
	if(s2 < s) {
		n += std::string().insert(0, s - s2, p);
	}
	return n;
}

class LogDelegateImpl : public LogDelegate {
public:
	LogDelegateImpl() :
		LogDelegate() {
	}

	virtual ~LogDelegateImpl() {
	}

	virtual void log(RuntimeRef runtime, LogSource source, LogType type, const std::string& clazz, const std::string& name, const std::string& message) {
		std::string s(source == Lib ? "Lib  " : "Host ");

#if OS_WINDOWS
		HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
		SetConsoleTextAttribute(h, 0x07);
		std::cerr << s;
		int c = 0x0f;
		switch (type) {
			case Debug:
				SetConsoleTextAttribute(h, 0x09);
				std::cerr << "Debug";
				c = 7;
				break;
			case Warn:
				SetConsoleTextAttribute(h, 0x0A);
				std::cerr << "Info ";
				break;
			case Info:
				SetConsoleTextAttribute(h, 0x0E);
				std::cerr << "Warn ";
				break;
			case Error:
				SetConsoleTextAttribute(h, 0x0C);
				std::cerr << "Error";
				break;
			case StdOut:
				SetConsoleTextAttribute(h, 0x02);
				std::cerr << "Out  ";
				break;
			case StdErr:
				SetConsoleTextAttribute(h, 0x0C);
				std::cerr << "Err  ";
				break;
		}

		SetConsoleTextAttribute(h, 0x06);
		std::cerr << " [";
		SetConsoleTextAttribute(h, 0x07);
		std::cerr << pad(clazz + " " + name, 25, ' ');
		SetConsoleTextAttribute(h, 0x06);
		std::cerr << "] ";
		SetConsoleTextAttribute(h, c);
		std::cerr << message << std::endl;
#else
		const char* l;
		const char* c;
		switch (type) {
			case Debug:
				l = DEBUG_MSG;
				c = COLOR_0;
				break;
			case Warn:
				l = WARN_MSG;
				c = COLOR_1;
				break;
			case Info:
				l = INFO_MSG;
				c = COLOR_1;
				break;
			case Error:
				l = ERROR_MSG;
				c = COLOR_1;
				break;
			case StdOut:
				l = STDOUT_MSG;
				c = COLOR_1;
				break;
			case StdErr:
				l = STDERR_MSG;
				c = COLOR_1;
				break;
		}

		std::ostringstream o;
		o << COLOR_SRC << s << l
			<< COLOR_SEP << " ["
			<< COLOR_SRC << pad(clazz + " " + name, 25, ' ')
			<< COLOR_SEP << "] "
			<< c << message
			<< COLOR_0 << std::endl;
		std::cerr << o.str() << std::flush;
#endif
	}
};

LogDelegateRef newLogDelegate() {
	return LogDelegateRef(new LogDelegateImpl());
}

} // namespace impl

} // namespace Berkelium
