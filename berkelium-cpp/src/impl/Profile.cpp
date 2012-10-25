// Copyright (c) 2012 The Berkelium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "berkelium/Profile.hpp"

namespace Berkelium {

Profile::Profile() {
}

Profile::~Profile() {
}

namespace impl {

std::string dummy = "todo";

class ProfileImpl : public Profile {
public:
	ProfileImpl() {
	}

	~ProfileImpl() {
	}

	bool Profile::isInUse() {
		return false;
	}

	bool Profile::isFound() {
		return true;
	}

	bool Profile::isSameVersion() {
		return true;
	}

	bool Profile::isTooNew() {
		return false;
	}

	const std::string& Profile::getApplicationName() {
		return dummy;
	}

	const std::string& Profile::getProfileName() {
		return dummy;
	}

	const std::string& Profile::getProfilePath() {
		return dummy;
	}

	InstanceRef Profile::open() {
		return InstanceRef();
	}

};

ProfileRef newProfile() {
	return ProfileRef(new ProfileImpl());
}

} // namespace impl

} // namespace Berkelium
