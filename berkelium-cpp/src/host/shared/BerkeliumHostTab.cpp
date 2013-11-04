// Copyright (c) 2013 The Berkelium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "BerkeliumHostDelegate.hpp"
#include "BerkeliumHostTab.hpp"
#include "BerkeliumHost.hpp"

#include <Berkelium/API/Update.hpp>
#include <Berkelium/API/Logger.hpp>
#include <Berkelium/IPC/Message.hpp>
#include <Berkelium/IPC/Channel.hpp>

namespace Berkelium {

using Ipc::CommandId;
using Ipc::ChannelRef;
using Ipc::LinkRef;
using Ipc::MessageRef;

namespace impl {

class BerkeliumHostTabImpl : public BerkeliumHostTab {
private:
	BerkeliumHostWindowWRef window;
	LoggerRef logger;
	ChannelRef recv;
	ChannelRef send;
	void* nativeWindow;
	void* nativeTab;

	BerkeliumHostTabImpl(BerkeliumHostWindowRef window, LoggerRef logger, ChannelRef ipc) :
		BerkeliumHostTab(),
		window(window),
		logger(logger),
		recv(ipc),
		send(ipc->getReverseChannel()),
		nativeWindow(window->getNative()),
		nativeTab() {
		fprintf(stderr, "new BerkeliumHostTabImpl\n");
	}

public:
	virtual ~BerkeliumHostTabImpl() {
		fprintf(stderr, "delete BerkeliumHostTabImpl\n");
		BerkeliumHostDelegate::destroyTab(nativeWindow, nativeTab);
	}

	static BerkeliumHostTabRef create(BerkeliumHostWindowRef window, LoggerRef logger, ChannelRef ipc) {
		BerkeliumHostTabImpl* impl = new BerkeliumHostTabImpl(window, logger, ipc);
		BerkeliumHostTabRef ret(impl);
		ipc->registerCallback(ret);
		impl->nativeTab = BerkeliumHostDelegate::createTab(impl->nativeWindow, ret);
		return ret;
	}

	virtual void onChannelDataReady(ChannelRef channel, MessageRef msg) {
		fprintf(stderr, "BerkeliumHostTabImpl::onDataReady\n");
		switch(CommandId cmd = msg->get_cmd()) {
			default: {
				logger->error() << "received unknown command '" << cmd << "'" << std::endl;
				break;
			}
		}
	}

	virtual void sendOnReady() {
		Berkelium::Ipc::MessageRef msg(Ipc::Message::create(logger));
		msg->add_cmd(CommandId::onReady);
		send->send(msg);
	}
};

} // namespace impl

BerkeliumHostTab::BerkeliumHostTab()
{
}

BerkeliumHostTab::~BerkeliumHostTab()
{
}

BerkeliumHostTabRef BerkeliumHostTab::createBerkeliumHostTab(BerkeliumHostWindowRef window, LoggerRef logger, ChannelRef ipc)
{
	return impl::BerkeliumHostTabImpl::create(window, logger, ipc);
}

} // namespace Berkelium

