// Copyright (c) 2012 The Berkelium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "BerkeliumHost.hpp"
#include "BerkeliumTab.hpp"

#include "content/public/browser/browser_thread.h"
#include "base/message_loop.h"

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <Berkelium/API/Logger.hpp>
#include <Berkelium/API/Util.hpp>
#include <Berkelium/IPC/Channel.hpp>
#include <Berkelium/IPC/Message.hpp>
#include <Berkelium/Impl/Impl.hpp>

#include <set>
#include <iostream>

namespace Berkelium {

using Ipc::CommandId;
using Ipc::Channel;
using Ipc::ChannelRef;
using Ipc::Message;
using Ipc::MessageRef;

LoggerRef logger;
ChannelRef ipc;
MessageRef msg;

std::set<BerkeliumTab*> allWindows;

ChannelRef BerkeliumHost::addWindow(BerkeliumTab* window) {
	ChannelRef ret(ipc->createSubChannel());
	fprintf(stderr, "berkelium tab added!\n");
	allWindows.insert(window);
	msg->reset();
	msg->add_str("addWindow");
	msg->add_str(ret->getName());
	//std::cerr << "send: addWindow" << " " << ret->getName() << std::endl;
	ipc->send(msg);

	return ret;
}

void BerkeliumHost::removeWindow(BerkeliumTab* window) {
	allWindows.erase(window);
	if(allWindows.empty()) {
		fprintf(stderr, "last berkelium tab closed!\n");
		content::BrowserThread::UnsafeGetMessageLoopForThread(content::BrowserThread::UI)->QuitWhenIdle();
	} else {
		fprintf(stderr, "closed berkelium tab!\n");
	}
}

void update() {
	static bool started_send = false;
	bool running = true;

	if(!started_send) {
		started_send = true;
		msg->add_str("berkelium");
		ipc->send(msg);
	}

	// call update on all tabs
	{
		for(std::set<BerkeliumTab*>::iterator it = allWindows.begin(); it != allWindows.end(); it++) {
			(*it)->Update();
		}
	}

	if(!ipc->isEmpty()) {
		ipc->recv(msg);
		bool sendAck = false;

		switch(CommandId cmd = msg->get_cmd()) {
			default: {
				logger->error() << "received unknown command '" << cmd << "'" << std::endl;
				sendAck = true;
				break;
			}

			case CommandId::exitHost: {
				fprintf(stderr, "berkelium update done!\n");
				for(std::set<BerkeliumTab*>::iterator it = allWindows.begin(); it != allWindows.end(); it++) {
					(*it)->Close();
				}
				running = false;
				sendAck = true;
				break;
			}

			case CommandId::createTab: {
				ChannelRef tab(ipc->createSubChannel());
				ChannelRef tab2(tab->getReverseChannel());
				//channels.push_back(tab);
				msg->reset();
				msg->add_str(tab->getName());
				ipc->send(msg);
				logger->info() << "created new tab with id " << tab->getName() << "!" << std::endl;

				// wait 2s and send onReady
				//MessageRef m = Message::create(logger);
				//m->add_cmd(CommandId::onReady);
				//todo.push(Entry(logger, 2000, tab2, m));
				//msg->reset();
				break;
			}

			case CommandId::createWindow: {
				bool incognito = msg->get_8() == 1;
				ChannelRef win(ipc->createSubChannel());
				//channels.push_back(win);
				msg->reset();
				msg->add_str(win->getName());
				ipc->send(msg);
				logger->info() << "created new " << (incognito ? "incognito" : "default") << " window with id "
						<< win->getName() << "!" << std::endl;
				break;
			}

			case CommandId::navigate: {
				std::string url(msg->get_str());
				logger->info() << "navigate to '" << url << "'" << std::endl;
				break;
			}
		}
		if(sendAck) {
			msg->reset();
			ipc->send(msg); // ACK
		}
	}

	if(!running) {
		return;
	}

	content::BrowserThread::PostDelayedTask(content::BrowserThread::UI, FROM_HERE, base::Bind(&update), base::TimeDelta::FromMilliseconds(10));

}

static int initialised = 0;

bool BerkeliumHost::init(const std::string& dir, const std::string& name) {
	if(initialised != 0) {
		fprintf(stderr, "berkelium init double call!\n");
	} else {
		initialised = 1;
		Berkelium::impl::enableBerkeliumHostMode();
		logger = Berkelium::Util::createRootLogger(NULL);
		ipc = Channel::getChannel(logger, dir, name, false);
		msg = ipc->getMessage();
		return true;
		/*
		int p = atoi(port.c_str());
		fifo_out = open(FIFO_OUT_NAME, O_WRONLY);
		if(fifo_out == -1) {
			fprintf(stderr, "can't open '" FIFO_OUT_NAME "'!\n");
			exit(0);
		}
		fifo_in = open(FIFO_IN_NAME, O_RDONLY | O_NONBLOCK);
		if(fifo_in == -1) {
			fprintf(stderr, "can't open '" FIFO_IN_NAME "'!\n");
			close(fifo_out);
			exit(0);
		}
		IpcSocket* sock = new IpcSocket();
		int code = sock->connect("127.0.0.1", p);
		if(code <= 0) {
			fprintf(stderr, "berkelium: failed to connect to port %d: %d\n", p, code);
			exit(0);
		}
		//fprintf(stderr, "berkelium: connected to port %d!\n", p);
		ipcSocket = sock;
		packetWriter = new PacketWriter(ipcSocket, 1000);
		*/
	}
	return false;
}

void BerkeliumHost::lasyInit() {
	if(initialised == 1) {
		initialised = 2;
		//fprintf(stderr, "berkelium update loop started!\n");
		update();
	} else if(initialised == 2) {
		// everything is ok
		//fprintf(stderr, "berkelium: running!\n");
	} else {
		fprintf(stderr, "berkelium init error!\n");
	}
}

void BerkeliumHost::destory() {
	ipc.reset();
	fprintf(stderr, "berkelium closed!\n");
}

bool BerkeliumHost::isActive() {
	return initialised != 0;
}

} // namespace Berkelium
