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

HostExecutableRef newHostExecutable(const std::string&);
InstanceRef newInstance(HostExecutableRef executable, ProfileRef profile, Ipc::ChannelRef ipc, ProcessRef process);

} // namespace impl

} // namespace Berkelium

#endif // BERKELIUM_IMPL_IMPL_HPP_
