// Copyright (c) 2013 The Berkelium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BERKELIUM_IMPL_MANAGER_HPP_
#define BERKELIUM_IMPL_MANAGER_HPP_
#pragma once

#include <Berkelium/API/Berkelium.hpp>
#include <Berkelium/Impl/Manager.hpp>

namespace Berkelium {

namespace impl {

class Manager {
protected:
	Manager();

public:
	virtual ~Manager() = 0;

	virtual void registerHostVersion(HostVersionRef version) = 0;
	virtual void unregisterHostVersion() = 0;
	virtual HostVersionRef getHostVersion(void* id) = 0;

	virtual void registerRuntime(RuntimeRef runtime) = 0;
	virtual void unregisterRuntime() = 0;
	virtual RuntimeRef getRuntime(void* id) = 0;

	virtual void registerHostExecutable(HostExecutableRef executable) = 0;
	virtual void unregisterHostExecutable() = 0;
	virtual HostExecutableRef getHostExecutable(void* id) = 0;

	virtual void registerProfile(ProfileRef profile) = 0;
	virtual void unregisterProfile() = 0;
	virtual ProfileRef getProfile(void* id) = 0;

	virtual void registerInstance(InstanceRef instance) = 0;
	virtual void unregisterInstance() = 0;
	virtual InstanceRef getInstance(void* id) = 0;

	virtual void registerWindow(WindowRef window) = 0;
	virtual void unregisterWindow() = 0;
	virtual WindowRef getWindow(void* id) = 0;

	virtual void registerTab(TabRef tab) = 0;
	virtual void unregisterTab() = 0;
	virtual TabRef getTab(void* id) = 0;

	virtual void registerLogger(LoggerRef logger) = 0;
	virtual void unregisterLogger() = 0;
	virtual LoggerRef getLogger(void* id) = 0;

	virtual bool locked(void* id) = 0;

	virtual void lock(void* id, void* obj) = 0;

	virtual void* unlock(void* id) = 0;
};

} // namespace impl

} // namespace Berkelium

#endif // BERKELIUM_IMPL_MANAGER_HPP_
