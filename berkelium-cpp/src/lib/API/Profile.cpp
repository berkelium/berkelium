// Copyright (c) 2012 The Berkelium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Berkelium/API/BerkeliumFactory.hpp>
#include <Berkelium/API/Profile.hpp>
#include <Berkelium/API/Util.hpp>
#include <Berkelium/Impl/Impl.hpp>

#include <cstdlib>
#include <iostream>
#include <sstream>
#include <fstream>
#include <boost/filesystem.hpp>

#ifdef LINUX
#include <unistd.h>
#include <limits.h>
#include <signal.h>
#endif

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
	bool locked;
	const bool temp;

public:
	ProfileImpl(const boost::filesystem::path& path, const std::string& application, const bool temp) :
		path(path),
		pathstr(path.string()),
		application(application),
		locked(false),
		temp(temp)
#ifdef LINUX
		, warned(false)
#endif
	{
		boost::filesystem::create_directories(path);
	}

	~ProfileImpl() {
		if(isLocked()) {
			setLocked(false);
		}
		if(temp && !application.empty() && !isInUse()) {
			std::cerr << "removing temporary profile " << path << std::endl;
			boost::filesystem::remove_all(path);
		}
	}

#ifdef LINUX
private:
	bool warned;

	boost::filesystem::path getLock() {
		return path / "SingletonLock";
	}

	std::string getHostname() {
		char tmp[HOST_NAME_MAX];
		gethostname(tmp, HOST_NAME_MAX);
		return std::string(tmp);
	}

public:
#endif

	bool isInUse() {
#ifdef WIN32
		boost::filesystem::path lock = path / "lockfile";
		if(!boost::filesystem::exists(lock)) {
			return false;
		}
		std::ofstream file(lock.string());
		return !file.is_open();
#elif defined(LINUX)
		boost::system::error_code ec;
		boost::filesystem::path read = boost::filesystem::read_symlink(getLock(), ec);
		if(ec) {
			warned = false;
			return false;
		}

		std::string s(read.string());
	    std::replace(s.begin(), s.end(), '-', ' ');
		std::istringstream is(s);
		std::string host;
		is >> host;
		int pid;
		is >> pid;

		//std::cerr << lock << " host '" << host << "' pid'" << pid << "'" << std::endl;

		std::string hostname(getHostname());
		if(hostname.compare(host) != 0) {
			if(!warned) {
				warned = true;
				std::cerr << "Warning: Profile " << path << " socket is from another host ('" << hostname << "' != '" << host <<  "')!" << std::endl;
			}
			return false;
		}

		if(kill(pid, 0) != 0) {
			if(!warned) {
				warned = true;
				std::cerr << "Warning: Profile " << path << " was not properly closed!?" << std::endl;
			}
			return false;
		}

		// TODO check if name of process contains berkelium...

		warned = false;

		return true;
#else
#error "TODO"
#endif
	}

	void setLocked(bool locked) {
		if(this->locked == locked) {
			return;
		}
		if(locked) {
			if(isInUse()) {
				return;
			}
#ifdef WIN32
#error "TODO"
#elif defined(LINUX)
			if(!isFound()) {
				boost::filesystem::create_directories(path);
			}
			std::ostringstream os;
			os << getHostname() << "-" << getpid();
			boost::filesystem::path to(os.str());
			boost::filesystem::create_symlink(to, getLock());
#else
#error "TODO"
#endif
		} else {
#ifdef WIN32
#error "TODO"
#elif defined(LINUX)
			boost::filesystem::remove(getLock());
#else
#error "TODO"
#endif
		}
		this->locked = locked;
	}

	bool isLocked() {
		return locked;
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
	path = Util::getEnv("LOCALAPPDATA", "C:");
	path /= appDir;
	path /= "User Data";
#elif defined(LINUX)
	path = Util::getEnv("HOME", "/tmp");
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
	path /= "berkelium." + Util::getEnv("USER", "user");
#else
#error "please add path to temp here"
#endif

	path /= "berkelium";
	impl::cleanup(path);
	path /= Util::randomId();

	return ProfileRef(new impl::ProfileImpl(path, "berkelium", true));
}

} // namespace Berkelium
