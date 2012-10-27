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

std::string toVersionString(int32_t vmajor, int32_t vminor, int32_t build, int32_t patch) {
	std::stringstream ret;
	ret << vmajor << '.' << vminor << '.' << build << '.' << patch;
	return ret.str();
}

class HostVersionImpl : public HostVersion {
private:
	const int32_t vmajor;
	const int32_t vminor;
	const int32_t build;
	const int32_t patch;

	const std::string version;

public:
	HostVersionImpl(int32_t vmajor, int32_t vminor, int32_t build, int32_t patch)
		: vmajor(vmajor)
		, vminor(vminor)
		, build(build)
		, patch(patch)
		, version(toVersionString(vmajor, vminor, build, patch))
	{
	}

	virtual ~HostVersionImpl() {
	}

	virtual const std::string& getVersionString() {
		return version;
	}

	virtual int32_t getMajor() {
		return vmajor;
	}

	virtual int32_t getMinor() {
		return vminor;
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

HostVersionRef newHostVersion(int32_t vmajor, int32_t vminor, int32_t build, int32_t patch) {
	return HostVersionRef(new HostVersionImpl(vmajor, vminor, build, patch));
}

HostVersionRef newHostVersion(const std::string& version) {
	HostVersionRef ret;
	std::stringstream ss(version);
	int8_t dot;

	int32_t vmajor;
	int32_t vminor;
	int32_t build;
	int32_t patch;

	if((ss >> vmajor).fail()) {
		return ret;
	}
	if((ss >> dot).fail() || dot != '.') {
		return ret;
	}
	if((ss >> vminor).fail()) {
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

	return newHostVersion(vmajor, vminor, build, patch);
}

} // namespace impl

} // namespace Berkelium
