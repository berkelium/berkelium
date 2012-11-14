// Copyright (c) 2012 The Berkelium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BERKELIUM_IMPL_HPP_
#define BERKELIUM_IMPL_HPP_
#pragma once

#ifndef BERKELIUM_CPP_IMPL
# error "This file is intended for internal use only!"
#endif

#include <Berkelium/API/Berkelium.hpp>

namespace Berkelium {

namespace impl {

class Ipc;
typedef std::shared_ptr<Ipc> IpcRef;

class IpcMessage;
typedef std::shared_ptr<IpcMessage> IpcMessageRef;

class Pipe;
typedef std::shared_ptr<Pipe> PipeRef;

class Process;
typedef std::shared_ptr<Process> ProcessRef;

typedef std::shared_ptr<int8_t> ByteRef;
typedef std::shared_ptr<int32_t> Int32Ref;

std::string randomId();
std::string randomId(int length);
HostExecutableRef newHostExecutable(const std::string&);
InstanceRef newInstance(HostExecutableRef executable, ProfileRef profile, IpcRef ipc, ProcessRef process);
void sleep(int32_t ms);
std::string getEnv(const std::string&, const std::string& defaultValue);
#ifdef WIN32
std::string ws2s(const std::wstring&);
std::wstring s2ws(const std::string&);
#endif

} // namespace impl

} // namespace Berkelium

#endif // BERKELIUM_IMPL_HPP_
