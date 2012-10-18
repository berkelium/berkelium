// Copyright (c) 2012 The Berkelium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "berkelium/HostExecutableFactory.hpp"
#include "berkelium/HostExecutable.hpp"
#include "Impl.hpp"

namespace Berkelium {

HostExecutableRef HostExecutableFactory::forExecutable(const std::string& pathTo) {
	return HostExecutableRef(impl::newHostExecutable());
}

HostExecutableRef HostExecutableFactory::forSystemInstalled() {
	return HostExecutableRef(impl::newHostExecutable());
}

} // namespace Berkelium
