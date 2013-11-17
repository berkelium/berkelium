// Copyright (c) 2012 The Berkelium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Berkelium/IPC/Link.hpp>
#include <Berkelium/IPC/Message.hpp>
#include <Berkelium/API/Util.hpp>
#include <Berkelium/Impl/Filesystem.hpp>

#include "gtest/gtest.h"
#include "test.h"

using Berkelium::Ipc::Link;
using Berkelium::Ipc::LinkRef;
using Berkelium::Ipc::LinkGroupRef;
using Berkelium::Ipc::Message;
using Berkelium::Ipc::MessageRef;
using Berkelium::Util::randomId;
using Berkelium::impl::Filesystem;

namespace {

class LinkTest : public ::testing::Test {
    DEFINE_LOGGER(LinkTest);
};

TEST_F(LinkTest, notCreateClient) {
    USE_LOGGER(create);
    ASSERT_NULL(Link::getLink(false, LinkGroupRef(), logger, Filesystem::getTemp(), randomId(), "testNotCreateClientLink"));
}

TEST_F(LinkTest, create) {
    USE_LOGGER(create);
    ASSERT_NOT_NULL(Link::getLink(true, LinkGroupRef(), logger, Filesystem::getTemp(), randomId(), "testCreateLink"));
}

TEST_F(LinkTest, remove) {
    USE_LOGGER(remove);
    std::string dir = Filesystem::append(Filesystem::getTemp(), randomId());
    std::string name(randomId());
    ASSERT_FALSE(Filesystem::exists(dir));
    {
        LinkRef pipe = Link::getLink(true, LinkGroupRef(), getLogger("remove"), dir, name, "testRemoveLink");
        ASSERT_TRUE(Filesystem::exists(dir));
        ASSERT_TRUE(Filesystem::exists(Filesystem::append(dir, name + "1")));
    }
    ASSERT_FALSE(Filesystem::exists(Filesystem::append(dir, name)));
    Filesystem::removeDir(dir);
}

TEST_F(LinkTest, sendRecv) {
    USE_LOGGER(sendRecv);
    std::string dir = Filesystem::append(Filesystem::getTemp(), randomId());
    std::string name(randomId());
    ASSERT_FALSE(Filesystem::exists(dir));
    {
        LinkRef link1 = Link::getLink(true, LinkGroupRef(), getLogger("sendRecv"), dir, name, "testSendRecvLink");
        ASSERT_TRUE(Filesystem::exists(dir));
        ASSERT_TRUE(Filesystem::exists(Filesystem::append(dir, name + "1")));
        ASSERT_TRUE(Filesystem::exists(Filesystem::append(dir, name + "2")));
        LinkRef link2 = Link::getLink(false, LinkGroupRef(), getLogger("sendRecv"), dir, name, "testSendRecvLink");
        ASSERT_TRUE(link1->isEmpty());
        ASSERT_TRUE(link2->isEmpty());
        MessageRef msg(Message::create(logger));
        msg->add_str("hello");
        link1->send(msg);
        ASSERT_TRUE(link1->isEmpty());
        ASSERT_FALSE(link2->isEmpty());
        msg = link2->recv();
        std::string recv = msg->get_str();
        ASSERT_TRUE(link1->isEmpty());
        ASSERT_TRUE(link2->isEmpty());
        ASSERT_EQ(0, recv.compare("hello"));
    }
    ASSERT_FALSE(Filesystem::exists(Filesystem::append(dir, name)));
    Filesystem::removeDir(dir);
}

#ifdef OS_WINDOWS
TEST_F(LinkTest, createClientServer) {
    USE_LOGGER(sendRecv);
    std::string name("testPipe");
    {
        LinkRef server = Link::getLink(true, LinkGroupRef(), getLogger("sendRecv"), "", name, "testSendRecvLink");
        ASSERT_TRUE(server->isEmpty());
        LinkRef client = Link::getLink(false, LinkGroupRef(), getLogger("sendRecv"), "", name, "testSendRecvLink");
        ASSERT_TRUE(client->isEmpty());
        MessageRef msg(Message::create(logger));
        msg->add_str("hello");
        server->send(msg);
        ASSERT_TRUE(server->isEmpty());
        ASSERT_FALSE(client->isEmpty());
        msg = client->recv();
        std::string recv = msg->get_str();
        ASSERT_TRUE(server->isEmpty());
        ASSERT_TRUE(client->isEmpty());
        ASSERT_EQ(0, recv.compare("hello"));
    }
}
#endif
} // namespace
