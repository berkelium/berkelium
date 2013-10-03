// Copyright (c) 2013 The Berkelium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "BerkeliumHost.hpp"
#include "BerkeliumHostInstance.hpp"
#include "BerkeliumHostWindow.hpp"

#include <Berkelium/API/Logger.hpp>
#include <Berkelium/IPC/Message.hpp>
#include <Berkelium/IPC/PipeGroup.hpp>
#include <Berkelium/IPC/Channel.hpp>

#include <set>

namespace Berkelium {

using Ipc::CommandId;
using Ipc::ChannelRef;
using Ipc::PipeGroupRef;
using Ipc::PipeRef;
using Ipc::MessageRef;

namespace impl {

class BerkeliumHostInstanceImpl : public BerkeliumHostInstance {
private:
	std::set<BerkeliumHostWindowRef> windows;
	LoggerRef logger;
	PipeGroupRef group;
	ChannelRef ipc;
	MessageRef msg;

	BerkeliumHostInstanceImpl(LoggerRef logger, PipeGroupRef group, ChannelRef ipc) :
		BerkeliumHostInstance(),
		windows(),
		logger(logger),
		group(group),
		ipc(ipc),
		msg(Ipc::Message::create(logger)) {
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

	static BerkeliumHostInstanceRef create(LoggerRef logger, PipeGroupRef group, ChannelRef ipc) {
		BerkeliumHostInstanceImpl* impl = new BerkeliumHostInstanceImpl(logger, group, ipc);
		BerkeliumHostInstanceRef ret(impl);
		group->registerCallback(ipc, ret, true);
		return ret;
	}

	virtual void onDataReady(PipeRef pipe) {
		pipe->recv(msg);
		fprintf(stderr, "BerkeliumHostInstanceImpl::onDataReady\n");
		switch(CommandId cmd = msg->get_cmd()) {
			case CommandId::createWindow: {
				onCreateWindow();
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

	void onCreateWindow() {
		bool incognito = msg->get_8() == 1;
		ChannelRef win(ipc->createSubChannel("window"));

		windows.insert(BerkeliumHostWindow::createBerkeliumHostWindow(logger, group, win, incognito));

		msg->reset();
		msg->add_str(win->getName());
		ipc->send(msg);

		logger->info() << "created new " << (incognito ? "incognito" : "default") << " window with id "
				<< win->getName() << "!" << std::endl;
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

BerkeliumHostInstanceRef BerkeliumHostInstance::createBerkeliumHostInstance(LoggerRef logger, PipeGroupRef group, ChannelRef ipc)
{
	return impl::BerkeliumHostInstanceImpl::create(logger, group, ipc);
}

} // namespace Berkelium
