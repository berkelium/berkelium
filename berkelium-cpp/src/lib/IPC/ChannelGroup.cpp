// Copyright (c) 2012 The Berkelium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Berkelium/IPC/ChannelGroup.hpp>
#include <Berkelium/IPC/Ipc.hpp>
#include <Berkelium/Impl/Impl.hpp>

namespace Berkelium {

namespace Ipc {

namespace impl {

class ChannelGroupImpl : public ChannelGroup {
public:
	ChannelGroupImpl() :
		ChannelGroup() {
	}

	virtual ~ChannelGroupImpl() {
	}

	virtual void registerChannel(ChannelRef channel) {
		Berkelium::impl::bk_error("register channel %p", channel.get());
	}

	virtual void unregisterChannel(Channel* channel) {
		Berkelium::impl::bk_error("unregister channel %p", channel);
	}
};

} // namespace impl

ChannelGroup::ChannelGroup() {
}

ChannelGroup::~ChannelGroup() {
}

ChannelGroupRef ChannelGroup::create() {
	return ChannelGroupRef(new impl::ChannelGroupImpl());
}

} // namespace Ipc

} // namespace Berkelium
