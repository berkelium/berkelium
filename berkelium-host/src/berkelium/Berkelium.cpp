// Copyright (c) 2012 The Berkelium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "Berkelium.hpp"
#include "BerkeliumTab.hpp"

#include "content/public/browser/browser_thread.h"
#include "base/message_loop.h"

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <Berkelium/IPC/Ipc.hpp>
#include <Berkelium/IPC/IpcMessage.hpp>

#include <set>
#include <iostream>

namespace Berkelium {

using ::Berkelium::impl::Ipc;
using ::Berkelium::impl::IpcRef;
using ::Berkelium::impl::IpcMessage;
using ::Berkelium::impl::IpcMessageRef;

IpcRef ipc;
IpcMessageRef msg = IpcMessage::create();

std::set<BerkeliumTab*> allWindows;

IpcRef Berkelium::addWindow(BerkeliumTab* window) {
	IpcRef ret(ipc->createChannel());
	fprintf(stderr, "berkelium tab added!\n");
	allWindows.insert(window);
	msg->reset();
	msg->add_str("addWindow");
	msg->add_str(ret->getName());
	//std::cerr << "send: addWindow" << " " << ret->getName() << std::endl;
	ipc->send(msg);

	return ret;
}

void Berkelium::removeWindow(BerkeliumTab* window) {
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
		std::string cmd = msg->get_str();
		std::cout << "recv: '" << cmd << "'" << std::endl;
		msg->reset();
		ipc->send(msg); // ACK

		if(cmd.compare("exit") == 0){
			fprintf(stderr, "berkelium update done!\n");
			for(std::set<BerkeliumTab*>::iterator it = allWindows.begin(); it != allWindows.end(); it++) {
				(*it)->Close();
			}
			return;
		}
	}

	content::BrowserThread::PostDelayedTask(content::BrowserThread::UI, FROM_HERE, base::Bind(&update), base::TimeDelta::FromMilliseconds(10));

}

static int initialised = 0;

bool Berkelium::init(const std::string& dir, const std::string& name) {
	if(initialised != 0) {
		fprintf(stderr, "berkelium init double call!\n");
	} else {
		initialised = 1;
		ipc = Ipc::getIpc(dir, name, false);
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

void Berkelium::lasyInit() {
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

void Berkelium::destory() {
	ipc.reset();
	fprintf(stderr, "berkelium closed!\n");
}

bool Berkelium::isActive() {
	return initialised != 0;
}

} // namespace Berkelium
