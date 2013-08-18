// Copyright (c) 2012 The Berkelium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Berkelium/API/BerkeliumFactory.hpp>
#include <Berkelium/API/HostVersion.hpp>
#include <Berkelium/API/Runtime.hpp>
#include <Berkelium/Impl/Impl.hpp>

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
BERKELIUM_IMPL_CLASS(HostVersion)

private:
	const int32_t vmajor;
	const int32_t vminor;
	const int32_t build;
	const int32_t patch;

	const std::string version;

public:
	HostVersionImpl(RuntimeRef runtime, int32_t vmajor, int32_t vminor, int32_t build, int32_t patch) :
		BERKELIUM_IMPL_CTOR1(HostVersion),
		vmajor(vmajor),
		vminor(vminor),
		build(build),
		patch(patch),
		version(toVersionString(vmajor, vminor, build, patch)) {
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

ManagerRef getManager(HostVersionRef version)
{
	if(!version) {
		return ManagerRef();
	}
	HostVersionImpl* impl = (HostVersionImpl*)version.get();
	return impl->getManager();
}

HostVersionRef newVersion(RuntimeRef runtime, int32_t vMajor, int32_t vMinor, int32_t vBuild, int32_t vPatch) {
	return HostVersionRef(new impl::HostVersionImpl(runtime, vMajor, vMinor, vBuild, vPatch));
}

HostVersionRef newVersion(RuntimeRef runtime, const std::string& version) {
	HostVersionRef ret;
	std::stringstream ss(version);
	int8_t dot;

	int32_t vMajor;
	int32_t vMinor;
	int32_t vBuild;
	int32_t vPatch;

	if((ss >> vMajor).fail()) {
		return ret;
	}
	if((ss >> dot).fail() || dot != '.') {
		return ret;
	}
	if((ss >> vMinor).fail()) {
		return ret;
	}
	if((ss >> dot).fail() || dot != '.') {
		return ret;
	}
	if((ss >> vBuild).fail()) {
		return ret;
	}
	if((ss >> dot).fail() || dot != '.') {
		return ret;
	}
	if((ss >> vPatch).fail()) {
		return ret;
	}

	if(!ss.eof()) {
		return ret;
	}

	return newVersion(runtime, vMajor, vMinor, vBuild, vPatch);
}

} // namespace impl

} // namespace Berkelium
