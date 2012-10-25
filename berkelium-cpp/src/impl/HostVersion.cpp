// Copyright (c) 2012 The Berkelium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "berkelium/HostVersion.hpp"

namespace Berkelium {

const std::string version = "0.0.0.0";

HostVersion::HostVersion() {
}

HostVersion::~HostVersion() {
}

namespace impl {

class HostVersionImpl : public HostVersion {
private:
	const int32_t major;
	const int32_t minor;
	const int32_t build;
	const int32_t patch;

public:
	HostVersionImpl(int32_t major, int32_t minor, int32_t build, int32_t patch)
		: major(major)
		, minor(minor)
		, build(build)
		, patch(patch)
	{
	}

	virtual ~HostVersionImpl() {
	}

	virtual const std::string& HostVersionImpl::getVersionString() {
		return version;
	}

	virtual int32_t HostVersionImpl::getMajor() {
		return major;
	}

	virtual int32_t HostVersionImpl::getMinor() {
		return minor;
	}

	virtual int32_t HostVersionImpl::getBuild() {
		return build;
	}

	virtual int32_t HostVersionImpl::getPatch() {
		return patch;
	}

	virtual bool HostVersionImpl::isMinVersion(const std::string& version) {
		return false;
	}

};

HostVersionRef newHostVersion(int32_t major, int32_t minor, int32_t build, int32_t patch) {
	return HostVersionRef(new HostVersionImpl(major, minor, build, patch));
}

} // namespace impl

} // namespace Berkelium
