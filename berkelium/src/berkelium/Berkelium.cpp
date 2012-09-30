// Copyright (c) 2012 The Berkelium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "Berkelium.hpp"

#include "content/public/browser/browser_thread.h"

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "../include/IpcSender.hpp"
#include "../include/PacketWriter.hpp"
#include "../include/IpcSocket.hpp"

#include <poll.h>

namespace Berkelium {

PacketWriter* packetWriter = NULL;
IpcSender* ipcSender = NULL;

void update() {
	/*
	fd_set rfds;
	FD_ZERO(&rfds);
	FD_SET(fifo_in, &rfds);

	struct timeval tv;
	tv.tv_sec = 0;
	tv.tv_usec = 0;
	int retval = select(fifo_in + 1, &rfds, NULL, NULL, &tv);
	if (retval == -1)
		perror("select()");
	else if (retval) {
		size_t size = read(fifo_in, &data, 1024);
		write(fifo_out, data, size);

	}
	*/
	pollfd cinfd[1];
	int c = cinfd[0].fd = fileno(stdin);
	cinfd[0].events = POLLIN;
	if(poll(cinfd, 1, 100)) {
		printf("read: %c\n", fgetc(stdin));
	}
	if (feof(stdin)){
		printf("done!\n");
	}
	printf("...\n");
	content::BrowserThread::PostDelayedTask(content::BrowserThread::UI, FROM_HERE, base::Bind(&update), base::TimeDelta::FromMilliseconds(10));

}

static int initialised = 0;

void Berkelium::init(const std::string& host, const std::string& port,const std::string& key) {
	if(initialised != 0) {
		fprintf(stderr, "berkelium init double call!\n");
	} else {
		initialised = 1;
		int p = atoi(port.c_str());
		fprintf(stderr, "berkelium started on port %d!\n", p);
		/*
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
		*/
		IpcSocket* sock = new IpcSocket();
		int code = sock->connect("127.0.0.1", p);
		if(code <= 0) {
			fprintf(stderr, "failed to connect to port %d: %d\n", p, code);
			exit(0);
		}
		ipcSender = sock;
		packetWriter = new PacketWriter(ipcSender, 1000);
	}
}

void Berkelium::lasyInit() {
	if(initialised == 1) {
		initialised = 2;
		fprintf(stderr, "berkelium update loop started!\n");
		update();
	} else if(initialised == 2) {
		// everything is ok
	} else {
		fprintf(stderr, "berkelium init error!\n");
	}
}

void Berkelium::destory() {
	fprintf(stderr, "berkelium closed!\n");
	delete packetWriter;
	delete ipcSender;
	packetWriter = NULL;
	ipcSender = NULL;
}

bool Berkelium::isActive() {
	return initialised != 0;
}

PacketWriter* Berkelium::getPacketWriter() {
	return packetWriter;
}

} // namespace Berkelium
