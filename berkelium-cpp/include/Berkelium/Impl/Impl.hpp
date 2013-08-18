// Copyright (c) 2012 The Berkelium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BERKELIUM_IMPL_IMPL_HPP_
#define BERKELIUM_IMPL_IMPL_HPP_
#pragma once

#ifndef BERKELIUM_CPP_IMPL
# error "This file is intended for internal use only!"
#endif

#include <Berkelium/API/Berkelium.hpp>
#include <Berkelium/IPC/Ipc.hpp>
#include <Berkelium/Impl/Process.hpp>

namespace Berkelium {

namespace impl {

#if 0
typedef std::shared_ptr<int8_t> ByteRef;
typedef std::shared_ptr<int32_t> Int32Ref;
#endif

class Manager;
typedef std::shared_ptr<Manager> ManagerRef;
typedef std::weak_ptr<Manager> ManagerWRef;

ManagerRef getManager(void* ptr);
ManagerRef getManager(HostVersionRef id);
ManagerRef getManager(RuntimeRef id);
ManagerRef getManager(HostExecutableRef id);
ManagerRef getManager(ProfileRef id);
ManagerRef getManager(LoggerRef id);
ManagerRef getManager(InstanceRef id);
ManagerRef getManager(WindowRef id);
ManagerRef getManager(TabRef id);
ManagerRef getManager(TabDelegateRef id);

void enableBerkeliumHostMode();
TabRef newTab(WindowRef window, Ipc::ChannelRef ipc);
WindowRef newWindow(InstanceRef instance, Ipc::ChannelRef channel, bool incognito);
HostExecutableRef newHostExecutable(RuntimeRef runtime, const std::string&);
InstanceRef newInstance(RuntimeRef runtime, HostExecutableRef executable, ProfileRef profile);
InstanceRef newInstance(HostExecutableRef executable, ProfileRef profile, Ipc::ChannelRef ipc, ProcessRef process);
HostVersionRef newVersion(RuntimeRef runtime, int32_t vMajor, int32_t vMinor, int32_t vBuild, int32_t vPatch);
HostVersionRef newVersion(RuntimeRef runtime, const std::string& version);
ManagerRef newManager(LogDelegateRef log);
ProfileRef newProfile(RuntimeRef runtime, const std::string& application);
ProfileRef getChromeProfile(RuntimeRef runtime);
ProfileRef getChromiumProfile(RuntimeRef runtime);
ProfileRef forProfilePath(RuntimeRef runtime, const std::string& path);
ProfileRef createTemporaryProfile(RuntimeRef runtime);
LoggerRef newLogger(LogDelegateRef target, RuntimeRef runtime, const std::string& clazz, const std::string& name);
LogDelegateRef newLogDelegate();
void setLoggerPrefix(RuntimeRef runtime, const std::string& name);

#define BERKELIUM_IMPL_CLASS(TypeName)			\
private:										\
	TypeName##Impl(const TypeName##Impl&);		\
	void operator=(const TypeName##Impl&);	\
	const RuntimeRef runtime;					\
	const ManagerRef manager;					\
	const LoggerRef logger;						\
												\
public:											\
	ManagerRef getManager() {					\
		return manager;							\
	}											\
	virtual RuntimeRef getRuntime() {			\
		return runtime;							\
	}

#define _BERKELIUM_IMPL_CTOR(TypeName, name, rt)		\
	TypeName(),											\
	runtime(rt),										\
	manager(Berkelium::impl::getManager(runtime)),		\
	logger(runtime->getLogger("##TypeName##", name))

#define BERKELIUM_IMPL_CTOR1(TypeName)			\
	_BERKELIUM_IMPL_CTOR(TypeName, "", runtime)

#define BERKELIUM_IMPL_CTOR2(TypeName, name)			\
	_BERKELIUM_IMPL_CTOR(TypeName, name, runtime)

#define BERKELIUM_IMPL_CTOR3(TypeName, name, rt)		\
	_BERKELIUM_IMPL_CTOR(TypeName, name, rt->getRuntime())

} // namespace impl

} // namespace Berkelium

#endif // BERKELIUM_IMPL_IMPL_HPP_
