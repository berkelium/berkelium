// Copyright (c) 2012 The Berkelium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "berkelium/HostExecutableFactory.hpp"
#include "berkelium/HostExecutable.hpp"
#include "berkelium/Impl.hpp"

namespace Berkelium {

HostExecutableRef HostExecutableFactory::forExecutable(const std::string& pathTo) {
	return HostExecutableRef(impl::newHostExecutable(pathTo));
}

HostExecutableRef HostExecutableFactory::forSystemInstalled() {
	std::string path;

#ifdef WIN32
	path = Berkelium::impl::getEnv("PROGRAMFILES(X86)") + "\\Google\\Chrome\\chrome.exe";
#else
#error "please add path to chrome here"
#endif

	return HostExecutableRef(impl::newHostExecutable(path));
}

} // namespace Berkelium
