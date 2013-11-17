// Copyright (c) 2013 The Berkelium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BERKELIUM_IPC_LINK_GROUP_HPP_
#define BERKELIUM_IPC_LINK_GROUP_HPP_
#pragma once

#ifndef BERKELIUM_CPP_IMPL
# error "This file is intended for internal use only!"
#endif

#include <Berkelium/IPC/Ipc.hpp>

namespace Berkelium {

namespace Ipc {

#ifdef WINDOWS
typedef void* LinkFdType;
#endif
#ifdef LINUX
typedef int LinkFdType;
#endif

class LinkGroup {
protected:
	LinkGroup();

public:
	virtual ~LinkGroup() = 0;

	virtual void registerLink(LinkRef pipe) = 0;

	virtual void unregisterLink(Link* pipe) = 0;

	virtual void registerCallback(ChannelGroupRef group, LinkCallbackRef callback) = 0;

	virtual void registerCallback(LinkRef pipe, LinkCallbackRef callback) = 0;

	virtual void update(int32_t timeout) = 0;

	virtual int64_t getLastRecv() = 0;

	static LinkGroupRef create();
};

} // namespace Ipc

} // namespace Berkelium

#endif // BERKELIUM_IPC_LINK_GROUP_HPP_
