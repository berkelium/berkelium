// Copyright (c) 2012 The Berkelium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BERKELIUM_IPC_IPC_HPP_
#define BERKELIUM_IPC_IPC_HPP_
#pragma once

#ifndef BERKELIUM_CPP_IMPL
# error "This file is intended for internal use only!"
#endif

#include <Berkelium/API/Berkelium.hpp>

namespace Berkelium {

namespace Ipc {

#ifdef LINUX
class PipePosix;
typedef std::shared_ptr<PipePosix> PipePosixRef;
typedef std::weak_ptr<PipePosix> PipePosixWRef;
#endif // LINUX

class LinkCallback;
typedef std::shared_ptr<LinkCallback> LinkCallbackRef;
typedef std::weak_ptr<LinkCallback> LinkCallbackWRef;

class ChannelCallback;
typedef std::shared_ptr<ChannelCallback> ChannelCallbackRef;
typedef std::weak_ptr<ChannelCallback> ChannelCallbackWRef;

class Channel;
typedef std::shared_ptr<Channel> ChannelRef;
typedef std::weak_ptr<Channel> ChannelWRef;

class ChannelGroup;
typedef std::shared_ptr<ChannelGroup> ChannelGroupRef;
typedef std::weak_ptr<ChannelGroup> ChannelGroupWRef;

class LinkGroup;
typedef std::shared_ptr<LinkGroup> LinkGroupRef;
typedef std::weak_ptr<LinkGroup> LinkGroupWRef;

class Message;
typedef std::shared_ptr<Message> MessageRef;
typedef std::weak_ptr<Message> MessageWRef;

class Link;
typedef std::shared_ptr<Link> LinkRef;
typedef std::weak_ptr<Link> LinkWRef;

typedef enum {
	exitHost,
	ping,
	createWindow,
	createTab,
	onReady,
	navigate,
} CommandId;

} // namespace Ipc

} // namespace Berkelium

#endif // BERKELIUM_IPC_IPC_HPP_
