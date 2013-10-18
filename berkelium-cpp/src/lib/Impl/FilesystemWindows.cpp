// Copyright (c) 2013 The Berkelium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifdef OS_WINDOWS

#include <Berkelium/API/Util.hpp>
#include <Berkelium/Impl/Filesystem.hpp>

#include <windows.h>
#include <tchar.h>

#define BUFSIZE 1024

namespace Berkelium {

namespace impl {
    
extern const char seperatorChar = '\\';
extern const std::string seperator = "\\";

std::string Filesystem::append(const std::string& dir, const std::string& append) {
    if (dir.empty()) {
        return append;
    }
	return dir + seperator + append;
}

std::string Filesystem::absolute(const std::string& arg) {
    TCHAR buffer[BUFSIZE];
    if(GetFullPathName(arg.c_str(), BUFSIZE, buffer, NULL) == 0) {
        // TODO error handling?
        return "";
    }
    return buffer;
}

std::string Filesystem::getTemp() {
    return append(Util::getEnv("TEMP", "C:\\WINDOWS\\TEMP"), "berkelium." + Util::getEnv("USERNAME", "User"));
}

void Filesystem::createDirectory(const std::string& dir) {
    CreateDirectory(dir.c_str(), NULL);
}

void Filesystem::removeFile(const std::string& file) {
    DeleteFile(file.c_str());
}

void Filesystem::removeEmptyDir(const std::string& dir) {
    RemoveDirectory(dir.c_str());
}

bool Filesystem::exists(const std::string& file) {
   WIN32_FIND_DATA findFileData;
   HANDLE handle = FindFirstFile(file.c_str(), &findFileData);
   bool found = handle != INVALID_HANDLE_VALUE;
   if (found) {
       FindClose(handle);
   }
   return found;
}

bool Filesystem::readDirectory(const std::string& dir, std::vector<std::string>& content) {
    const std::string path = dir + "\\*.*";
    
    WIN32_FIND_DATA fdFile;
    HANDLE hFind = FindFirstFile(path.c_str(), &fdFile);
    if(hFind == INVALID_HANDLE_VALUE) {
        return false;
    }

    do {
        if(strcmp(fdFile.cFileName, ".") == 0 || strcmp(fdFile.cFileName, "..") == 0) {
            continue;
        }
        content.push_back(fdFile.cFileName);
    } while(FindNextFile(hFind, &fdFile));

    FindClose(hFind);
    return true;
}

} // namespace impl

} // namespace Berkelium

#endif // OS_WINDOWS
