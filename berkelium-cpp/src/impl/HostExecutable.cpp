// Copyright (c) 2012 The Berkelium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "berkelium/BerkeliumFactory.hpp"
#include "berkelium/HostExecutable.hpp"
#include "berkelium/Profile.hpp"
#include "berkelium/Impl.hpp"

#include <boost/filesystem.hpp>

namespace Berkelium {

HostExecutable::HostExecutable() {
}

HostExecutable::~HostExecutable() {
}

namespace impl {

HostVersionRef getVersionImpl(const boost::filesystem::path&);

class HostExecutableImpl : public HostExecutable {
private:
	const boost::filesystem::path& path;
	HostVersionRef version;

public:
	HostExecutableImpl(const boost::filesystem::path& path)
		: path(path) {
	}

	virtual ~HostExecutableImpl() {
	}

	virtual HostVersionRef getVersion() {
		if(!version) {
			version = getVersionImpl(path);
		}
		return version;
	}
};

HostExecutableRef newHostExecutable(const boost::filesystem::path& path) {
	return HostExecutableRef(new HostExecutableImpl(path));
}

HostExecutableRef newHostExecutable(const std::string& path) {
	return newHostExecutable(boost::filesystem::path(path));
}

HostVersionRef getVersionImpl(const boost::filesystem::path&) {
	HostVersionRef ret;
	ret = Berkelium::BerkeliumFactory::forVersion("0.0.0.0");
	return ret;
}

} // namespace impl

} // namespace Berkelium
