// Copyright (c) 2012 The Berkelium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Berkelium/API/Runtime.hpp>
#include <Berkelium/API/HostExecutable.hpp>
#include <Berkelium/API/Profile.hpp>
#include <Berkelium/API/Logger.hpp>
#include <Berkelium/Impl/Impl.hpp>

#include <boost/filesystem.hpp>

namespace Berkelium {

HostExecutable::HostExecutable() {
}

HostExecutable::~HostExecutable() {
}

namespace impl {

HostVersionRef getVersionImpl(RuntimeRef runtime, const boost::filesystem::path&);

class HostExecutableImpl : public HostExecutable {
BERKELIUM_IMPL_CLASS(HostExecutable)

private:
	const boost::filesystem::path path;
	const std::string pathStr;
	HostVersionRef version;

public:
	HostExecutableImpl(RuntimeRef runtime, const boost::filesystem::path& path) :
		BERKELIUM_IMPL_CTOR1(HostExecutable),
		path(path),
		pathStr(path.string()) {
	}

	virtual ~HostExecutableImpl() {
	}

	virtual const std::string& getPath() {
		return pathStr;
	}

	virtual HostVersionRef getVersion() {
		if(!version) {
			version = getVersionImpl(runtime, path);
		}
		return version;
	}
};

HostExecutableRef newHostExecutable(RuntimeRef runtime, const boost::filesystem::path& path) {
	return HostExecutableRef(new HostExecutableImpl(runtime, path));
}

HostExecutableRef newHostExecutable(RuntimeRef runtime, const std::string& path) {
	return newHostExecutable(runtime, boost::filesystem::path(path));
}

HostVersionRef getVersionImpl(RuntimeRef runtime, const boost::filesystem::path&) {
	HostVersionRef ret;
	ret = runtime->forVersion("0.0.0.0");
	return ret;
}

} // namespace impl

} // namespace Berkelium
