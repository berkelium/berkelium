// Copyright (c) 2012 The Berkelium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "berkelium/Profile.hpp"
#include "berkelium/Impl.hpp"

#include <iostream>
#include <fstream>
#include <boost/filesystem.hpp>

namespace Berkelium {

Profile::Profile() {
}

Profile::~Profile() {
}

namespace impl {

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
		return true;
	}

	bool isTooNew() {
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

	InstanceRef open() {
		return InstanceRef();
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

} // namespace Berkelium
