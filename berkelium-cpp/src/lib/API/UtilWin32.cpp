// Copyright (c) 2012 The Berkelium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifdef WIN32

#include <Berkelium/API/Util.hpp>

#include <vector>

#include <windows.h>
#include <time.h>

namespace Berkelium {

namespace Util {

void initRandom() {
	static bool initialised = false;
	if(!initialised) {
		initialised = true;
		srand((unsigned) time(NULL));
	}
}

int64_t currentTimeMillis() {
	FILETIME wt;
	GetSystemTimeAsFileTime(&wt);
	int64_t ret = (int64_t)wt.dwHighDateTime << 32;
	ret += wt.dwLowDateTime;
	ret -= 116444736000000000;
	ret /= 1000;
	return ret;
}

std::wstring getEnv(const std::wstring& env, const std::wstring& defaultValue) {
	DWORD bufferSize = 65535;
	std::wstring buffer;
	buffer.resize(bufferSize);
	DWORD length = GetEnvironmentVariableW(env.c_str(), &buffer[0], bufferSize);
	if(length == 0) {
		return defaultValue;
	}
	buffer.resize(length);
	return buffer;
}

std::string getEnv(const std::string& env, const std::string& defaultValue) {
	return ws2s(getEnv(s2ws(env), s2ws(defaultValue)));
}

std::string ws2s(const std::wstring& from) {
	if(from.empty()) {
        return std::string();
    }

	int length = ::WideCharToMultiByte(0, 0, from.data(), from.length(), NULL, 0, NULL, NULL);
    if(length == 0) {
        return std::string();
    }

    std::vector<char> buffer(length);
	::WideCharToMultiByte(0, 0, from.data(), from.length(), &buffer[0], length, NULL, NULL);

    return std::string(buffer.begin(), buffer.end());
}

std::wstring s2ws(const std::string& from) {
	if(from.empty()) {
        return std::wstring();
    }

	int length = ::MultiByteToWideChar(CP_ACP, 0, from.data(), from.length(), NULL, 0);
    if(length == 0) {
        return std::wstring();
    }

    std::vector<wchar_t> buffer(length);
	::MultiByteToWideChar(CP_ACP, 0, from.data(), from.length(), &buffer[0], length);

	return std::wstring(buffer.begin(), buffer.end());
}

void sleep(int32_t ms) {
	::Sleep(ms);
}

} // namespace Util

} // namespace Berkelium

#endif // WIN32
