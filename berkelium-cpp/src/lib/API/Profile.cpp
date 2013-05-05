// Copyright (c) 2012 The Berkelium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Berkelium/API/BerkeliumFactory.hpp>
#include <Berkelium/API/Profile.hpp>
#include <Berkelium/API/Util.hpp>
#include <Berkelium/API/Runtime.hpp>
#include <Berkelium/API/Logger.hpp>
#include <Berkelium/Impl/Impl.hpp>
#include <Berkelium/Impl/Filesystem.hpp>

#include <algorithm>
#include <cstdlib>
#include <sstream>
#include <fstream>

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
BERKELIUM_IMPL_CLASS(Profile)

private:
	const std::string path;
	const std::string application;
	bool locked;
	const bool temp;

public:
	ProfileImpl(RuntimeRef runtime, const std::string& path, const std::string& application, const bool temp) :
		BERKELIUM_IMPL_CTOR1(Profile),
		path(path),
		application(application),
		locked(false),
		temp(temp)
#ifdef LINUX
		, warned(false)
#endif
	{
		Filesystem::createDirectories(path);
	}

	virtual ~ProfileImpl() {
		if(isLocked()) {
			setLocked(false);
		}
		if(temp && !application.empty() && !isInUse()) {
			logger->debug() << "removing temporary profile " << path << std::endl;
			Filesystem::removeDir(path);
		}
	}

#ifdef LINUX
private:
	bool warned;

	std::string getLock() {
		return Filesystem::append(path, "SingletonLock");
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
		std::string lock(Filesystem::append(path, "lockfile"));
		if(!Filesystem::exists(lock)) {
			return false;
		}
		std::ofstream file(lock);
		return !file.is_open();
#elif defined(LINUX)
		std::string s;
		if(!Filesystem::readSymlink(getLock(), s)) {
			warned = false;
			return false;
		}

	    std::replace(s.begin(), s.end(), '-', ' ');
		std::istringstream is(s);
		std::string host;
		is >> host;
		int pid;
		is >> pid;

		//logger->debug() << lock << " host '" << host << "' pid'" << pid << "'" << std::endl;

		std::string hostname(getHostname());
		if(hostname.compare(host) != 0) {
			if(!warned) {
				warned = true;
				logger->warn() << "Profile " << path << " socket is from another host ('" << hostname << "' != '" << host <<  "')!" << std::endl;
			}
			return false;
		}

		if(kill(pid, 0) != 0) {
			if(!warned) {
				warned = true;
				logger->warn() << "Profile " << path << " was not properly closed!?" << std::endl;
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
				Filesystem::createDirectories(path);
			}
			std::ostringstream os;
			os << getHostname() << "-" << getpid();
			Filesystem::createSymlink(getLock(), os.str());
#else
#error "TODO"
#endif
		} else {
#ifdef WIN32
#error "TODO"
#elif defined(LINUX)
			Filesystem::removeFile(getLock());
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
		return Filesystem::exists(path);
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
		return path;
	}
};

// see http://www.chromium.org/user-experience/user-data-directory (without "Default" at the end)
ProfileRef newProfile(RuntimeRef runtime, const std::string& appDir, const std::string& application) {
	std::string path;

#ifdef WIN32
	path = Filesystem::append(Util::getEnv("LOCALAPPDATA", "C:"), appDir, "User Data");
#elif defined(LINUX)
	path = Filesystem::append(Util::getEnv("HOME", "/tmp"), ".config", appDir);
#else
#error "please add path to chrome profile here"
#endif
	return ProfileRef(new impl::ProfileImpl(runtime, path, application, false));
}

void cleanup(RuntimeRef runtime, const std::string& tmp) {
	static bool done = false;
	if(done) return;
	done = true;
	std::vector<std::string> dir;
	if(Filesystem::readDirectory(tmp, dir)) {
		for(std::vector<std::string>::iterator it(dir.begin()); it != dir.end(); ++it) {
			// create temporary profile object
			// this will remove old profile directory
			// if they are not used
			ProfileImpl(runtime, *it, "berkelium", true);
		}
	}
}

ProfileRef newProfile(RuntimeRef runtime, const std::string& application) {
	return impl::newProfile(runtime, application, application);
}

ProfileRef getChromeProfile(RuntimeRef runtime) {
#ifdef WIN32
	return impl::newProfile(runtime, "Google\\Chrome", "Google Chrome");
#elif defined(LINUX)
	return impl::newProfile(runtime, "google-chrome", "Google Chrome");
#else
#error "please add app path to profile here"
#endif
}

ProfileRef getChromiumProfile(RuntimeRef runtime) {
#ifdef WIN32
	return impl::newProfile(runtime, "Chromium", "Chromium");
#elif defined(LINUX)
	return impl::newProfile(runtime, "chromium", "Chromium");
#else
#error "please add app path to profile here"
#endif
}

ProfileRef forProfilePath(RuntimeRef runtime, const std::string& path) {
	return ProfileRef(new impl::ProfileImpl(runtime, path, "berkelium", false));
}

ProfileRef createTemporaryProfile(RuntimeRef runtime) {
	std::string path;

#ifdef WIN32
	path = Util::getEnv("TEMP", "C:\\WINDOWS\\TEMP");
#elif defined(LINUX)
	path = Filesystem::append("/tmp", "berkelium." + Util::getEnv("USER", "user"));
#else
#error "please add path to temp here"
#endif

	path = Filesystem::append(path, "berkelium");
	impl::cleanup(runtime, path);
	path = Filesystem::append(path, Util::randomId());

	return ProfileRef(new impl::ProfileImpl(runtime, path, "berkelium", true));
}

} // namespace impl

} // namespace Berkelium
