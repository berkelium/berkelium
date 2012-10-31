// Copyright (c) 2012 The Berkelium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "berkelium/BerkeliumFactory.hpp"
#include "berkelium/Profile.hpp"
#include "berkelium/Impl.hpp"

#include <cstdlib>
#include <iostream>
#include <fstream>
#include <boost/filesystem.hpp>

namespace Berkelium {

Profile::Profile() {
}

Profile::~Profile() {
}

namespace impl {

std::string randomId(int length) {
	static const std::string CHARS("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890");
	const int to = CHARS.size();
	std::string ret;

	srand((unsigned)time(0));
	for(int i = 0; i < length; i++) {
		ret += CHARS[std::rand() % to];
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
			std::cerr << "removing temporary profile " << path << std::endl;
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
#elif defined(LINUX)
		boost::filesystem::path lock = path / "SingletonLock";
		boost::system::error_code ec;
		boost::filesystem::path read = boost::filesystem::read_symlink(lock, ec);
		if(!ec) {
			return true;
		}

		// TODO check for running instance is missing

		return false;
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
#elif defined(LINUX)
	path = impl::getEnv("HOME", "/tmp");
	path /= ".config";
	path /= appDir;
#else
#error "please add path to chrome profile here"
#endif
	return ProfileRef(new impl::ProfileImpl(path, application, false));
}

void cleanup(const boost::filesystem::path& tmp) {
	static bool done = false;
	if(done) return;
	done = true;
	if(boost::filesystem::is_directory(tmp)) {
		for(boost::filesystem::directory_iterator itr(tmp); itr != boost::filesystem::directory_iterator(); ++itr) {
			// create temporary profile object
			// this will remove old profile directory
			// if they are not used
			ProfileImpl(*itr, "berkelium", true);
		}
	}
}

} // namespace impl

ProfileRef BerkeliumFactory::forProfile(const std::string& application) {
	return impl::newProfile(application, application);
}

ProfileRef BerkeliumFactory::getChromeProfile() {
#ifdef WIN32
	return impl::newProfile("Google\\Chrome", "Google Chrome");
#elif defined(LINUX)
	return impl::newProfile("google-chrome", "Google Chrome");
#else
#error "please add app path to profile here"
#endif
}

ProfileRef BerkeliumFactory::getChromiumProfile() {
#ifdef WIN32
	return impl::newProfile("Chromium", "Chromium");
#elif defined(LINUX)
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
#elif defined(LINUX)
	path = "/tmp";
	path /= "berkelium." + impl::getEnv("USER", "user");
#else
#error "please add path to temp here"
#endif

	path /= "berkelium";
	impl::cleanup(path);
	path /= impl::randomId();

	return ProfileRef(new impl::ProfileImpl(path, "berkelium", true));
}

} // namespace Berkelium
