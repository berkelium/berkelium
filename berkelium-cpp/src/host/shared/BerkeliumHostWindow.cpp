// Copyright (c) 2013 The Berkelium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "BerkeliumHostDelegate.hpp"
#include "BerkeliumHostWindow.hpp"
#include "BerkeliumHostTab.hpp"

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

class BerkeliumHostWindowImpl : public BerkeliumHostWindow {
private:
	BerkeliumHostWindowWRef self;
	std::set<BerkeliumHostTabRef> tabs;
	LoggerRef logger;
	PipeGroupRef group;
	ChannelRef ipc;
	MessageRef msg;
	void* native;
	bool incognito;

	BerkeliumHostWindowImpl(LoggerRef logger, PipeGroupRef group, ChannelRef ipc, bool incognito) :
		BerkeliumHostWindow(),
		self(),
		tabs(),
		logger(logger),
		group(group),
		ipc(ipc),
		msg(Ipc::Message::create(logger)),
		native(BerkeliumHostDelegate::createWindow(incognito)),
		incognito(incognito) {
		fprintf(stderr, "new BerkeliumHostWindowImpl\n");
	}

public:
	virtual ~BerkeliumHostWindowImpl() {
		fprintf(stderr, "delete BerkeliumHostWindowImpl\n");
		for(std::set<BerkeliumHostTabRef>::iterator it(tabs.begin()); it != tabs.end(); it++) {
			BerkeliumHostTabRef tab(*it);
			tab.reset();
		}
		tabs.clear();
		BerkeliumHostDelegate::destroyWindow(native);
	}

	virtual void* getNative() {
		return native;
	}

	static BerkeliumHostWindowRef create(LoggerRef logger, PipeGroupRef group, ChannelRef ipc, bool incognito) {
		BerkeliumHostWindowImpl* impl = new BerkeliumHostWindowImpl(logger, group, ipc, incognito);
		BerkeliumHostWindowRef ret(impl);
		impl->self = ret;
		group->registerCallback(ipc, ret, false);
		return ret;
	}

	virtual void onDataReady(PipeRef pipe) {
		pipe->recv(msg);
		fprintf(stderr, "BerkeliumHostWindowImpl::onDataReady\n");
		switch(CommandId cmd = msg->get_cmd()) {
			case CommandId::createTab: {
				onCreateTab();
				break;
			}
			default: {
				logger->error() << "received unknown command '" << cmd << "'" << std::endl;
				break;
			}
		}
	}

	void onCreateTab() {
		ChannelRef tab(ipc->createSubChannel("tab"));

		tabs.insert(BerkeliumHostTab::createBerkeliumHostTab(self.lock(), logger, group, tab));

		msg->reset();
		msg->add_str(tab->getName());
		ipc->send(msg);

		logger->info() << "created new tab with id " << tab->getName() << "!" << std::endl;

		/*
		// wait 2s and send onReady
		MessageRef m = Message::create(logger);
		m->add_cmd(CommandId::onReady);
		todo.push(Entry(2000, tab2, m));
		msg->reset();
		break;
		*/
	}
};

} // namespace impl

BerkeliumHostWindow::BerkeliumHostWindow()
{
}

BerkeliumHostWindow::~BerkeliumHostWindow()
{
}

BerkeliumHostWindowRef BerkeliumHostWindow::createBerkeliumHostWindow(LoggerRef logger, PipeGroupRef group, ChannelRef ipc, bool incognito)
{
	return impl::BerkeliumHostWindowImpl::create(logger, group, ipc, incognito);
}

} // namespace Berkelium
