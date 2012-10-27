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

	bool isInUse() {
		return false;
	}

	bool isFound() {
		return true;
	}

	bool isSameVersion() {
		return true;
	}

	bool isTooNew() {
		return false;
	}

	const std::string& getApplicationName() {
		return dummy;
	}

	const std::string& getProfileName() {
		return dummy;
	}

	const std::string& getProfilePath() {
		return dummy;
	}

	InstanceRef open() {
		return InstanceRef();
	}

};

ProfileRef newProfile() {
	return ProfileRef(new ProfileImpl());
}

} // namespace impl

} // namespace Berkelium
