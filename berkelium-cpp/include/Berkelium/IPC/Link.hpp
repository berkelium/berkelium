// Copyright (c) 2012 The Berkelium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BERKELIUM_IPC_LINK_HPP_
#define BERKELIUM_IPC_LINK_HPP_
#pragma once

#ifndef BERKELIUM_CPP_IMPL
# error "This file is intended for internal use only!"
#endif

#include <Berkelium/IPC/Ipc.hpp>

namespace Berkelium {

namespace Ipc {

class LinkCallback
{
public:
	LinkCallback();

	virtual ~LinkCallback() = 0;

	virtual void onLinkDataReady(LinkRef pipe) = 0;
};

template<class T, class I>
class LinkCallbackDelegate : public LinkCallback {
private:
	std::weak_ptr<T> wref;

public:
	LinkCallbackDelegate(std::weak_ptr<T> wref) :
		LinkCallback(),
		wref(wref) {
	}

	virtual ~LinkCallbackDelegate() {
	}

	virtual void onLinkDataReady(LinkRef pipe) {
		std::shared_ptr<T> ref(wref.lock());
		if(ref) {
			I* impl = (I*)ref.get();
			impl->onLinkDataReady(pipe);
		}
	}
};

class Link {
protected:
	Link();

public:
	static LinkRef getLink(bool server, LinkGroupRef group, LoggerRef logger, const std::string& dir, const std::string& name, const std::string& alias);

	virtual ~Link() = 0;

	// Returns true if there are no pending messages to receive.
	virtual bool isEmpty() = 0;

	// Sends this message.
	virtual void send(MessageRef msg) = 0;

	// Receives the next message.
	virtual MessageRef recv() = 0;

	virtual const std::string getName() = 0;

	virtual const std::string getAlias() = 0;
};

} // namespace Ipc

namespace impl {

int getLinkFd(Ipc::LinkRef pipe);

} // namespace impl

} // namespace Berkelium

#endif // BERKELIUM_IPC_LINK_HPP_
