// Copyright (c) 2012 The Berkelium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "berkelium/BerkeliumFactory.hpp"
#include "berkelium/Profile.hpp"
#include "berkelium/Impl.hpp"

#include <iostream>
#include <fstream>
#include <boost/filesystem.hpp>
#include <boost/random/random_device.hpp>
#include <boost/random/uniform_int_distribution.hpp>

namespace Berkelium {

Profile::Profile() {
}

Profile::~Profile() {
}

namespace impl {

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

class ProfileImpl : public Profile {
private:
	const boost::filesystem::path path;
	const std::string pathstr;
	const std::string application;
	const std::string profile;
	const bool temp;
public:
	ProfileImpl(const boost::filesystem::path& path, const std::string& application, const std::string& profile, const bool temp) :
		path(path),
		pathstr(path.string()),
		application(application),
		profile(profile),
		temp(temp) {
	}

	~ProfileImpl() {
		if(temp && !application.empty() && !profile.empty() && !isInUse()) {
			boost::filesystem::remove_all(path);
		}
	}

	bool isInUse() {
#ifdef WIN32
		boost::filesystem::path lock = path / "lockfile";
		if(!boost::filesystem::exists(lock)) {
			return false;
		}
		std::ofstream file(lock.string());
		return !file.is_open();
#else
#error "TODO"
#endif
	}

	bool isFound() {
		return boost::filesystem::exists(path);
	}

	bool isSameVersion() {
		// TODO
		return true;
	}

	bool isTooNew() {
		// TODO
		return false;
	}

	const std::string& getApplicationName() {
		return application;
	}

	const std::string& getProfileName() {
		return profile;
	}

	const std::string& getProfilePath() {
		return pathstr;
	}
};

ProfileRef newProfile(const std::string& application, const std::string& profile, const bool temp) {
	boost::filesystem::path path;

#ifdef WIN32
	if(temp) {
		path = getEnv("TEMP", "C:\\WINDOWS\\TEMP");
	} else {
		path = getEnv("LOCALAPPDATA", "C:");
	}
#elif LINUX
	if(temp) {
		path = "~/.config";
	} else {
		path = "/tmp";
	}
#else
#error "please add path to chrome here"
#endif

	path /= application;
	path /= profile;

	return ProfileRef(new ProfileImpl(path, application, profile, temp));
}

} // namespace impl

ProfileRef BerkeliumFactory::forProfile(const std::string& application) {
	return forProfile(application, "Default");
}

ProfileRef BerkeliumFactory::forProfile(const std::string& application, const std::string& profile) {
	return impl::newProfile(application, profile, false);
}

ProfileRef BerkeliumFactory::forChromeProfile(const std::string& profile) {
	return ProfileRef();
}

ProfileRef BerkeliumFactory::forChromeProfile() {
	return ProfileRef();
}

ProfileRef BerkeliumFactory::createTemporaryProfile() {
	return impl::newProfile("berkelium", impl::randomId(), true);
}

} // namespace Berkelium
