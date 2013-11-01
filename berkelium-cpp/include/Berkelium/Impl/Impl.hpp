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

#include <map>

namespace Berkelium {

namespace impl {

#if 0
typedef std::shared_ptr<int8_t> ByteRef;
typedef std::shared_ptr<int32_t> Int32Ref;
#endif

class Process;
typedef std::shared_ptr<Process> ProcessRef;

class Manager;
typedef std::shared_ptr<Manager> ManagerRef;
typedef std::weak_ptr<Manager> ManagerWRef;

class UpdateQueue;
typedef std::shared_ptr<UpdateQueue> UpdateQueueRef;
typedef std::weak_ptr<UpdateQueue> UpdateQueueWRef;

typedef std::map<int64_t, UpdateRef> UpdateRefMap;
typedef std::pair<int64_t, UpdateRef> UpdateRefMapPair;
typedef UpdateRefMap::iterator UpdateRefMapIt;

ManagerRef getManager(HostVersion* id);
inline ManagerRef getManager(HostVersionRef id) {
	return getManager(id.get());
}

ManagerRef getManager(Runtime* id);
inline ManagerRef getManager(RuntimeRef id) {
	return getManager(id.get());
}

ManagerRef getManager(HostExecutable* id);
inline ManagerRef getManager(HostExecutableRef id) {
	return getManager(id.get());
}

ManagerRef getManager(Profile* id);
inline ManagerRef getManager(ProfileRef id) {
	return getManager(id.get());
}

ManagerRef getManager(Logger* id);
inline ManagerRef getManager(LoggerRef id) {
	return getManager(id.get());
}

ManagerRef getManager(Instance* id);
inline ManagerRef getManager(InstanceRef id) {
	return getManager(id.get());
}

ManagerRef getManager(Window* id);
inline ManagerRef getManager(WindowRef id) {
	return getManager(id.get());
}

ManagerRef getManager(Tab* id);
inline ManagerRef getManager(TabRef id) {
	return getManager(id.get());
}

void enableBerkeliumHostMode();
bool isBerkeliumHostMode();
const char* getBerkeliumHostMode();
TabRef newTab(WindowRef window, Ipc::ChannelRef ipc);
WindowRef newWindow(InstanceRef instance, Ipc::ChannelRef channel, bool incognito);
HostExecutableRef newHostExecutable(RuntimeRef runtime, const std::string&);
InstanceRef newInstance(RuntimeRef runtime, HostExecutableRef executable, ProfileRef profile);
InstanceRef newInstance(HostExecutableRef executable, ProfileRef profile, Ipc::ChannelRef ipc, ProcessRef process, Ipc::ChannelRef ping);
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
Ipc::ChannelRef createChannel(LoggerRef logger, int32_t id, Ipc::ChannelGroupRef group, const std::string& alias);
Ipc::LinkGroupRef getLinkGroup(RuntimeRef runtime);
void setLoggerPrefix(RuntimeRef runtime, const std::string& name);
Ipc::LinkRef getInputLink(Ipc::ChannelGroupRef group);

void testHostCrash(InstanceRef instance);
void testHostHang(InstanceRef instance);

void bk_error(const char* format, ...);

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
	logger(runtime->getLogger(#TypeName, name))

#define BERKELIUM_IMPL_CTOR1(TypeName)			\
	_BERKELIUM_IMPL_CTOR(TypeName, "", runtime)

#define BERKELIUM_IMPL_CTOR2(TypeName, name)			\
	_BERKELIUM_IMPL_CTOR(TypeName, name, runtime)

#define BERKELIUM_IMPL_CTOR3(TypeName, name, rt)		\
	_BERKELIUM_IMPL_CTOR(TypeName, name, rt->getRuntime())

#ifdef LINUX
int getLinkFd(Ipc::LinkRef pipe);
int getLinkFd(Ipc::PipePosixRef pipe);
#endif

#ifdef BERKELIUM_TRACE_ALL_OBJECTS
void updateCount(const char* name, bool up);
std::string getBerkeliumObjectCount();
#define TRACE_OBJECT_NEW(x) Berkelium::impl::updateCount(x, true)
#define TRACE_OBJECT_DELETE(x) Berkelium::impl::updateCount(x, false)
#else
inline std::string getBerkeliumObjectCount() {
	return "";
}
#define TRACE_OBJECT_NEW(x)
#define TRACE_OBJECT_DELETE(x)
#endif // BERKELIUM_TRACE_ALL_OBJECTS

} // namespace impl

} // namespace Berkelium

#include <Berkelium/Impl/Process.hpp>

#endif // BERKELIUM_IMPL_IMPL_HPP_
