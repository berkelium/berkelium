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
#include <Berkelium/IPC/LinkGroup.hpp>

#ifdef WINDOWS
// Check if we could skip this include here
// Only included here for the HANDLE type.
#include <Windows.h>
#endif

namespace Berkelium {

namespace Ipc {

class LinkCallback
{
public:
    LinkCallback();

    virtual ~LinkCallback() = 0;

    // The caller must ensure that 'data' is freed
    virtual void onLinkDataReady(LinkRef link, uint32_t size, uint8_t* data) = 0;
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

    virtual void onLinkDataReady(LinkRef link, uint32_t size, uint8_t* data) {
        std::shared_ptr<T> ref(wref.lock());
        if(ref) {
            I* impl = (I*)ref.get();
            impl->onLinkDataReady(link, size, data);
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

    virtual void waitForInit() = 0;

    // Sends this message.
    virtual void send(MessageRef msg) = 0;

#ifdef WINDOWS
    // TODO maybe add it to linux too ?
    virtual MessageRef recv(int32_t size) = 0;
#endif

    // Receives the next message.
    virtual MessageRef recv() = 0;

    virtual const std::string getName() = 0;

    virtual const std::string getAlias() = 0;
};

} // namespace Ipc

namespace impl {

Berkelium::Ipc::LinkFdType getLinkFd(Ipc::LinkRef pipe);

} // namespace impl

} // namespace Berkelium

#endif // BERKELIUM_IPC_LINK_HPP_
