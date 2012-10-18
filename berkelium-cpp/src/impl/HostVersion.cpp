// Copyright (c) 2012 The Berkelium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "berkelium/HostVersion.hpp"

namespace Berkelium {

const std::string version = "0.0.0.0";

const std::string& HostVersion::getVersionString() {
	return version;
}

int32_t HostVersion::getMajor() {
	return 0;
}

int32_t HostVersion::getMinor() {
	return 0;
}

int32_t HostVersion::getBuild() {
	return 0;
}

int32_t HostVersion::getPatch() {
	return 0;
}

bool HostVersion::isMinVersion(const std::string& version) {
	return false;
}

} // namespace Berkelium
