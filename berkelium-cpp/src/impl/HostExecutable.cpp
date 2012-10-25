// Copyright (c) 2012 The Berkelium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "berkelium/HostExecutable.hpp"
#include "berkelium/Profile.hpp"
#include "berkelium/Impl.hpp"

namespace Berkelium {

HostExecutable::HostExecutable() {
}

HostExecutable::~HostExecutable() {
}

namespace impl {

class HostExecutableImpl : public HostExecutable {
public:
	HostExecutableImpl() {
	}

	virtual ~HostExecutableImpl() {
	}

	virtual HostVersionRef getVersion() {
		return HostVersionRef(impl::newHostVersion(0, 0, 0, 0));
	}

	virtual ProfileRef forProfile(const std::string& application) {
		return ProfileRef(impl::newProfile());
	}

	virtual ProfileRef forProfile(const std::string& application, const std::string& profile) {
		return ProfileRef(impl::newProfile());
	}

	virtual ProfileRef createTemporaryProfile() {
		return ProfileRef(impl::newProfile());
	}
};

HostExecutableRef newHostExecutable() {
	return HostExecutableRef(new HostExecutableImpl());
}

} // namespace impl

} // namespace Berkelium
