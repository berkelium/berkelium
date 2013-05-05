// Copyright (c) 2012 The Berkelium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Berkelium/API/Runtime.hpp>
#include <Berkelium/API/HostExecutable.hpp>
#include <Berkelium/API/Profile.hpp>
#include <Berkelium/API/Logger.hpp>
#include <Berkelium/Impl/Impl.hpp>
#include <Berkelium/Impl/Filesystem.hpp>

namespace Berkelium {

HostExecutable::HostExecutable() {
}

HostExecutable::~HostExecutable() {
}

namespace impl {

HostVersionRef getVersionImpl(RuntimeRef runtime, const std::string&);

class HostExecutableImpl : public HostExecutable {
BERKELIUM_IMPL_CLASS(HostExecutable)

private:
	const std::string path;
	HostVersionRef version;

public:
	HostExecutableImpl(RuntimeRef runtime, const std::string& path) :
		BERKELIUM_IMPL_CTOR1(HostExecutable),
		path(path) {
	}

	virtual ~HostExecutableImpl() {
	}

	virtual const std::string& getPath() {
		return path;
	}

	virtual HostVersionRef getVersion() {
		if(!version) {
			version = getVersionImpl(runtime, path);
		}
		return version;
	}
};

HostExecutableRef newHostExecutable(RuntimeRef runtime, const std::string& path) {
	return HostExecutableRef(new HostExecutableImpl(runtime, path));
}

HostVersionRef getVersionImpl(RuntimeRef runtime, const std::string&) {
	HostVersionRef ret;
	ret = runtime->forVersion("0.0.0.0");
	return ret;
}

} // namespace impl

} // namespace Berkelium
