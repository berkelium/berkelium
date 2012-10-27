// Copyright (c) 2012 The Berkelium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "berkelium/HostVersion.hpp"

#include <sstream>

namespace Berkelium {

HostVersion::HostVersion() {
}

HostVersion::~HostVersion() {
}

namespace impl {

std::string toVersionString(int32_t major, int32_t minor, int32_t build, int32_t patch) {
	std::stringstream ret;
	ret << major << '.' << minor << '.' << build << '.' << patch;
	return ret.str();
}

class HostVersionImpl : public HostVersion {
private:
	const int32_t major;
	const int32_t minor;
	const int32_t build;
	const int32_t patch;

	const std::string version;

public:
	HostVersionImpl(int32_t major, int32_t minor, int32_t build, int32_t patch)
		: major(major)
		, minor(minor)
		, build(build)
		, patch(patch)
		, version(toVersionString(major, minor, build, patch))
	{
	}

	virtual ~HostVersionImpl() {
	}

	virtual const std::string& getVersionString() {
		return version;
	}

	virtual int32_t getMajor() {
		return major;
	}

	virtual int32_t getMinor() {
		return minor;
	}

	virtual int32_t getBuild() {
		return build;
	}

	virtual int32_t getPatch() {
		return patch;
	}

	virtual bool isMinVersion(const std::string& version) {
		return false;
	}

};

HostVersionRef newHostVersion(int32_t major, int32_t minor, int32_t build, int32_t patch) {
	return HostVersionRef(new HostVersionImpl(major, minor, build, patch));
}

HostVersionRef newHostVersion(const std::string& version) {
	HostVersionRef ret;
	std::stringstream ss(version);
	int8_t dot;

	int32_t major;
	int32_t minor;
	int32_t build;
	int32_t patch;

	if((ss >> major).fail()) {
		return ret;
	}
	if((ss >> dot).fail() || dot != '.') {
		return ret;
	}
	if((ss >> minor).fail()) {
		return ret;
	}
	if((ss >> dot).fail() || dot != '.') {
		return ret;
	}
	if((ss >> build).fail()) {
		return ret;
	}
	if((ss >> dot).fail() || dot != '.') {
		return ret;
	}
	if((ss >> patch).fail()) {
		return ret;
	}

	if(!ss.eof()) {
		return ret;
	}

	return newHostVersion(major, minor, build, patch);
}

} // namespace impl

} // namespace Berkelium
