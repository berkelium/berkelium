// Copyright (c) 2012 The Berkelium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "berkelium/BerkeliumFactory.hpp"
#include "berkelium/HostExecutable.hpp"
#include "berkelium/Impl.hpp"

namespace Berkelium {

HostExecutableRef BerkeliumFactory::forExecutable(const std::string& pathTo) {
	return HostExecutableRef(impl::newHostExecutable(pathTo));
}

HostExecutableRef BerkeliumFactory::forSystemInstalled() {
	std::string path;

#ifdef WIN32
	path = Berkelium::impl::getEnv("PROGRAMFILES(X86)", "C:\\Program Files") + "\\Google\\Chrome\\chrome.exe";
#elif LINUX
	path = "../berkelium-host/berkelium";
#else
#error "please add path to chrome here"
#endif

	return HostExecutableRef(impl::newHostExecutable(path));
}

InstanceRef BerkeliumFactory::open(HostExecutableRef executable, ProfileRef profile) {
	// TODO
	return InstanceRef();
}

} // namespace Berkelium
