// Copyright (c) 2012 The Berkelium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifdef WIN32

#include <Berkelium/API/Util.hpp>
#include <Berkelium/Impl/Impl.hpp>
#include <Berkelium/Impl/Process.hpp>
#include <Berkelium/IPC/LinkGroup.hpp>

#include <Windows.h>

#include <iostream>
#include <sstream>
#include <algorithm>
#include <iterator>

#ifdef BERKELIUM_NO_HOST_REDIRECT
#define redirect 0
#else
#define redirect 1
#endif

namespace Berkelium {

namespace impl {

class ProcessWindowsImpl : public Process {
private:
	STARTUPINFOW si;
	PROCESS_INFORMATION pi;
/*
	HANDLE hStdInput;
	HANDLE hStdOutout;
	HANDLE hStdError;
	bool stdOutDone;
	bool stdErrDone;
*/
	bool started;

public:
	ProcessWindowsImpl(RuntimeRef runtime, LoggerRef logger, const std::string& dir) :
		Process(runtime, logger, dir),
/*
		hStdInput(),
		hStdOutput(),
		hStdError),
		stdOutDone(false),
		stdErrDone(false),
*/
		started(false) {
		ZeroMemory(&si, sizeof(si));
		si.cb = sizeof(si);
/*
		if(redirect) {
			// See http://support.microsoft.com/kb/190351
			// TODO setup handles
			si.dwFlags = STARTF_USESTDHANDLES;
			si.hStdInput  = hStdInput;
			si.hStdOutput = hStdOutout;
			si.hStdError  = hStdError;
		}
*/
		ZeroMemory(&pi, sizeof(pi));
		TRACE_OBJECT_NEW("ProcessWin32");
	}

	virtual ~ProcessWindowsImpl() {
		TRACE_OBJECT_DELETE("ProcessWin32");
		if(!started) return;
		// Wait until child process exits.
		WaitForSingleObject(pi.hProcess, INFINITE);

		// Close process and thread handles. 
		CloseHandle(pi.hProcess);
		CloseHandle(pi.hThread);

		/*
		if(redirect) {
			// TODO close handles
		}
		*/
	}

	/*
	void update(bool& done, HANDLE handle, std::string& buffer, ConsoleRedirector& cr) {
		CHAR lpBuffer[256];
		DWORD nBytesRead;
		if(!done) {
			if (ReadFile(handle, lpBuffer, sizeof(lpBuffer), &nBytesRead,NULL) || !nBytesRead) {
				buffer += std::string(buf, 0, r);
				cr.update(buffer);
			} else {
				if (GetLastError() != ERROR_BROKEN_PIPE) {
					bk_error("ReadFile", "STDOUT");
				}
				done = true;
			}
		}
	}
	*/

	virtual void update() {
		/*
		update(&stdOutDone, hStdOutput, bufout, out);
		update(&stdErrDone, hStdError, buferr, err);
		*/
	}

	virtual bool isRunning() {
		DWORD exitCode;
		GetExitCodeProcess(pi.hProcess, &exitCode);
		return exitCode == STILL_ACTIVE;
	}

	virtual bool isCrashed() {
		DWORD exitCode;
		GetExitCodeProcess(pi.hProcess, &exitCode);
		return exitCode > 0 && exitCode != STILL_ACTIVE;
	}

	virtual const bool start(const std::vector<std::string>& args) {
		std::stringstream s;
		std::copy(args.begin(), args.end(), std::ostream_iterator<std::string>(s," "));
		std::wstring wcmd = Berkelium::Util::s2ws(s.str());
		std::wcerr << "wcmd:" << wcmd << std::endl;

		DWORD flags = 0;
		if(!redirect) {
			flags |= CREATE_NEW_CONSOLE;
		}

		// Start the child process. 
		if(!CreateProcessW(NULL, (LPWSTR)wcmd.c_str(), NULL, NULL, FALSE, flags, NULL, NULL, &si, &pi )) {
			bk_error("CreateProcess failed (%d).\n", GetLastError());
			return false;
		}

		group->waitForInit();

		started = true;
		return true;
	}
};

ProcessRef Process::create(RuntimeRef runtime, LoggerRef logger, const std::string& dir) {
	return ProcessRef(new ProcessWindowsImpl(runtime, logger, dir));
}

} // namespace impl

} // namespace Berkelium

#endif // WIN32
