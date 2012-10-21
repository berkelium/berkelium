// Copyright (c) 2012 The Berkelium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "berkelium/HostExecutable.hpp"
#include "berkelium/Profile.hpp"
#include "Impl.hpp"

namespace Berkelium {

HostExecutable::HostExecutable() {
}

HostExecutable::~HostExecutable() {
}

HostVersionRef HostExecutable::getVersion() {
	return HostVersionRef(impl::newHostVersion());
}

ProfileRef HostExecutable::forProfile(const std::string& application) {
	return ProfileRef(impl::newProfile());
}

ProfileRef HostExecutable::forProfile(const std::string& application, const std::string& profile) {
	return ProfileRef(impl::newProfile());
}

ProfileRef HostExecutable::createTemporaryProfile() {
	return ProfileRef(impl::newProfile());
}

} // namespace Berkelium
