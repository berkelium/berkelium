// Copyright (c) 2012 The Berkelium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifdef WIN32

#include "berkelium/Impl.hpp"
#include "Process.hpp"

#include <Windows.h>

#include <iostream>
#include <sstream>
#include <algorithm>
#include <iterator>

namespace Berkelium {

namespace impl {

class ProcessImpl : public Process {
private:
	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	bool started;

public:
	ProcessImpl() :
		started(false) {
		ZeroMemory(&si, sizeof(si));
		si.cb = sizeof(si);
		ZeroMemory(&pi, sizeof(pi));
	}

	virtual ~ProcessImpl() {
		if(!started) return;
		// Wait until child process exits.
		WaitForSingleObject(pi.hProcess, INFINITE);

		// Close process and thread handles. 
		CloseHandle(pi.hProcess);
		CloseHandle(pi.hThread);
	}

	virtual const bool start(const std::vector<std::string>& args) {
		std::stringstream s;
		std::copy(args.begin(), args.end(), std::ostream_iterator<std::string>(s," "));
		std::wstring wcmd = impl::s2ws(s.str());
		std::wcerr << "wcmd:" << wcmd << std::endl;

		// Start the child process. 
		if( !CreateProcess( NULL,   // No module name (use command line)
			(LPWSTR)wcmd.c_str(),   // Command line
			NULL,           // Process handle not inheritable
			NULL,           // Thread handle not inheritable
			FALSE,          // Set handle inheritance to FALSE
			0,              // No creation flags
			NULL,           // Use parent's environment block
			NULL,           // Use parent's starting directory 
			&si,            // Pointer to STARTUPINFO structure
			&pi )           // Pointer to PROCESS_INFORMATION structure
		) {
			printf( "CreateProcess failed (%d).\n", GetLastError() );
			return false;
		}

		started = true;
		return true;
	}
};

ProcessRef Process::create() {
	return ProcessRef(new ProcessImpl());
}

Process::~Process() {
}

} // namespace impl

} // namespace Berkelium

#endif // WIN32
