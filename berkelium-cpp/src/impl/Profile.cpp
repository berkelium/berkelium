// Copyright (c) 2012 The Berkelium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "berkelium/Profile.hpp"
#include "berkelium/Impl.hpp"

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
		path = getEnv("LOCALAPPDATA", "C:");
	} else {
		path = getEnv("TEMP", "C:\\WINDOWS\\TEMP");
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

	std::cerr << path << std::endl;

	return ProfileRef(new ProfileImpl(path, application, profile, temp));
}

} // namespace impl

} // namespace Berkelium
