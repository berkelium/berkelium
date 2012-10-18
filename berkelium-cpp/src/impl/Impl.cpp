// Copyright (c) 2012 The Berkelium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "berkelium/HostExecutable.hpp"
#include "berkelium/HostVersion.hpp"
#include "berkelium/Profile.hpp"

namespace Berkelium {

namespace impl {

class HostVersionImpl : public HostVersion {
};

HostVersionRef newHostVersion() {
	return HostVersionRef(new HostVersionImpl());
}

class HostExecutableImpl : public HostExecutable {
};

HostExecutableRef newHostExecutable() {
	return HostExecutableRef(new HostExecutableImpl());
}

class ProfileImpl : public Profile {
};

ProfileRef newProfile() {
	return ProfileRef(new ProfileImpl());
}

} // namespace impl

} // namespace Berkelium
