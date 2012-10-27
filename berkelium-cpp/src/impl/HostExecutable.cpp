// Copyright (c) 2012 The Berkelium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "berkelium/HostExecutable.hpp"
#include "berkelium/Profile.hpp"
#include "berkelium/Impl.hpp"

#include <boost/filesystem.hpp>
#include <boost/random/random_device.hpp>
#include <boost/random/uniform_int_distribution.hpp>

namespace Berkelium {

HostExecutable::HostExecutable() {
}

HostExecutable::~HostExecutable() {
}

namespace impl {

HostVersionRef getVersionImpl(const boost::filesystem::path&);

std::string randomId(int length) {
	static const std::string CHARS("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890");
	boost::random::uniform_int_distribution<> INDEX(0, CHARS.size() - 1);
	boost::random::random_device rnd;
	std::string ret;

	for(int i = 0; i < length; i++) {
		ret += CHARS[INDEX(rnd)];
	}

	return ret;
}

std::string randomId() {
	return randomId(8);
}

class HostExecutableImpl : public HostExecutable {
private:
	const boost::filesystem::path& path;
	HostVersionRef version;

public:
	HostExecutableImpl(const boost::filesystem::path& path)
		: path(path) {
	}

	virtual ~HostExecutableImpl() {
	}

	virtual HostVersionRef getVersion() {
		if(!version) {
			version = getVersionImpl(path);
		}
		return version;
	}

	virtual ProfileRef forProfile(const std::string& application) {
		return forProfile(application, "Default");
	}

	virtual ProfileRef forProfile(const std::string& application, const std::string& profile) {
		return ProfileRef(impl::newProfile(application, profile, false));
	}

	virtual ProfileRef createTemporaryProfile() {
		return ProfileRef(impl::newProfile("berkelium", randomId(), true));
	}
};

HostExecutableRef newHostExecutable(const boost::filesystem::path& path) {
	return HostExecutableRef(new HostExecutableImpl(path));
}

HostExecutableRef newHostExecutable(const std::string& path) {
	return newHostExecutable(boost::filesystem::path(path));
}

HostVersionRef getVersionImpl(const boost::filesystem::path&) {
	HostVersionRef ret;
	ret = newHostVersion("0.0.0.0");
	return ret;
}

} // namespace impl

} // namespace Berkelium
