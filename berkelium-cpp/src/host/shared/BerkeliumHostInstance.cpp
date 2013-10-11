// Copyright (c) 2013 The Berkelium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "BerkeliumHost.hpp"
#include "BerkeliumHostInstance.hpp"
#include "BerkeliumHostWindow.hpp"

#include <Berkelium/API/Logger.hpp>
#include <Berkelium/IPC/Message.hpp>
#include <Berkelium/IPC/Channel.hpp>
#include <Berkelium/IPC/ChannelGroup.hpp>

#include <set>

namespace Berkelium {

using Ipc::CommandId;
using Ipc::ChannelRef;
using Ipc::PipeRef;
using Ipc::MessageRef;

namespace impl {

class BerkeliumHostInstanceImpl : public BerkeliumHostInstance {
private:
	std::set<BerkeliumHostWindowRef> windows;
	LoggerRef logger;
	ChannelRef ipc;

	BerkeliumHostInstanceImpl(LoggerRef logger, ChannelRef ipc) :
		BerkeliumHostInstance(),
		windows(),
		logger(logger),
		ipc(ipc) {
		fprintf(stderr, "new BerkeliumHostInstanceImpl\n");
	}

public:
	virtual ~BerkeliumHostInstanceImpl() {
		fprintf(stderr, "delete BerkeliumHostInstanceImpl\n");
		/*
		for(std::set<BerkeliumHostWindowRef>::iterator it(windows.begin()); it != windows.end(); it++) {
			BerkeliumHostWindowRef window(*it);
			window.reset();
		}
		windows.clear();
		*/
	}

	static BerkeliumHostInstanceRef create(LoggerRef logger, ChannelRef ipc) {
		BerkeliumHostInstanceImpl* impl = new BerkeliumHostInstanceImpl(logger, ipc);
		BerkeliumHostInstanceRef ret(impl);
		ipc->registerCallback(ret);
		return ret;
	}

	virtual void onDataReady(ChannelRef channel, MessageRef msg) {
		fprintf(stderr, "BerkeliumHostInstanceImpl::onDataReady\n");
		switch(CommandId cmd = msg->get_cmd()) {
			case CommandId::createWindow: {
				onCreateWindow(msg);
				break;
			}
			case CommandId::exitHost: {
				onExitHost();
				break;
			}
			default: {
				logger->error() << "received unknown command '" << cmd << "'" << std::endl;
				break;
			}
		}
	}

	void onCreateWindow(MessageRef msg) {
		bool incognito = msg->get_8() == 1;
		ChannelRef win(ipc->getGroup()->createChannel("window"));

		windows.insert(BerkeliumHostWindow::createBerkeliumHostWindow(logger, win, incognito));

		msg->reset();
		msg->add_32(win->getId());
		ipc->send(msg);

		logger->info() << "created new " << (incognito ? "incognito" : "default") << " window with id "
				<< win->getId() << "!" << std::endl;
	}

	void onExitHost() {
		BerkeliumHost::setDone();
	}
};

} // namespace impl

BerkeliumHostInstance::BerkeliumHostInstance()
{
}

BerkeliumHostInstance::~BerkeliumHostInstance()
{
}

BerkeliumHostInstanceRef BerkeliumHostInstance::createBerkeliumHostInstance(LoggerRef logger, ChannelRef ipc)
{
	return impl::BerkeliumHostInstanceImpl::create(logger, ipc);
}

} // namespace Berkelium
