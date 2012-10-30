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
	const bool temp;
public:
	ProfileImpl(const boost::filesystem::path& path, const std::string& application, const bool temp) :
		path(path),
		pathstr(path.string()),
		application(application),
		temp(temp) {
	}

	~ProfileImpl() {
		if(temp && !application.empty() && !isInUse()) {
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

	const std::string& getProfilePath() {
		return pathstr;
	}
};

// see http://www.chromium.org/user-experience/user-data-directory (without "Default" at the end)
ProfileRef newProfile(const boost::filesystem::path& appDir, const std::string& application) {
	boost::filesystem::path path;

#ifdef WIN32
	path = impl::getEnv("LOCALAPPDATA", "C:");
	path /= appDir;
	path /= "User Data";
#elif LINUX
	path = impl::getEnv("HOME", "/tmp");
	path /= ".config";
	path /= appDir;
#else
#error "please add path to chrome profile here"
#endif
	return ProfileRef(new impl::ProfileImpl(path, application, false));
}

} // namespace impl

ProfileRef BerkeliumFactory::forProfile(const std::string& application) {
	return impl::newProfile(application, application);
}

ProfileRef BerkeliumFactory::getChromeProfile() {
#ifdef WIN32
	return impl::newProfile("Google\\Chrome", "Google Chrome");
#elif LINUX
	return impl::newProfile("google-chrome", "Google Chrome");
#else
#error "please add app path to profile here"
#endif
}

ProfileRef BerkeliumFactory::getChromiumProfile() {
#ifdef WIN32
	return impl::newProfile("Chromium", "Chromium");
#elif LINUX
	return impl::newProfile("chromium", "Chromium");
#else
#error "please add app path to profile here"
#endif
}

ProfileRef BerkeliumFactory::forProfilePath(const std::string& path) {
	return ProfileRef(new impl::ProfileImpl(path, "berkelium", false));
}

ProfileRef BerkeliumFactory::createTemporaryProfile() {
	boost::filesystem::path path;

#ifdef WIN32
	path = impl::getEnv("TEMP", "C:\\WINDOWS\\TEMP");
#elif LINUX
	path = "/tmp";
#else
#error "please add path to temp here"
#endif

	path /= "berkelium";
	path /= impl::randomId();

	return ProfileRef(new impl::ProfileImpl(path, "berkelium", true));
}

} // namespace Berkelium
