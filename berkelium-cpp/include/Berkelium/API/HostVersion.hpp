// Copyright (c) 2013 The Berkelium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BERKELIUM_API_HOSTVERSION_HPP_
#define BERKELIUM_API_HOSTVERSION_HPP_
#pragma once

// =========================================
// WARNING:
//   THIS FILE IS AUTOMATICALLY GENERATED!
//   !! ANY CHANGES WILL BE OVERWRITTEN !!
//
// See berkelium/berkelium-api/update.sh
// =========================================

#include <Berkelium/API/Berkelium.hpp>

namespace Berkelium {

class HostVersion {
protected:
	HostVersion();

public:
	virtual ~HostVersion() = 0;

	// Returns the associated Runtime.
	virtual RuntimeRef getRuntime() = 0;

	// Returns the berkelium host version e.g. “22.0.1229.94”
	virtual const std::string& getVersionString() = 0;

	// Returns the Major Version, e.g. “22”
	virtual int32_t getMajor() = 0;

	// Returns the Minor Version, always zero?
	virtual int32_t getMinor() = 0;

	// Returns the Build Version, e.g. “1229”
	virtual int32_t getBuild() = 0;

	// Returns the Patch Version, e.g. “94”
	virtual int32_t getPatch() = 0;

	// Returns true if the given Version is less or equal to the version of the executable represented by this executable object.
	virtual bool isMinVersion(const std::string& version) = 0;
};

} // namespace Berkelium

#endif // BERKELIUM_HOSTVERSION_HPP_
