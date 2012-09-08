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

namespace Berkelium {

#define FIFO_OUT_NAME "/tmp/berkelium.out.fifo"
#define FIFO_IN_NAME "/tmp/berkelium.in.fifo"

#define BERKELIUM_WRITE(x) write(fifo_out, x, strlen(x));

int fifo_in;
int fifo_out;
char data[1024];

void update() {
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
	content::BrowserThread::PostDelayedTask(content::BrowserThread::UI, FROM_HERE, base::Bind(&update), 10);

}

static int initialised = 0;

void Berkelium::init() {
	if(initialised != 0) {
		fprintf(stderr, "berkelium init double call!\n");
		BERKELIUM_WRITE("berkelium init double call!\n");
	} else {
		initialised = 1;
		fprintf(stderr, "berkelium started!\n");
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
		BERKELIUM_WRITE("berkelium started!\n");
	}
}

void Berkelium::lasyInit() {
	if(initialised == 1) {
		initialised = 2;
		fprintf(stderr, "berkelium update loop started!\n");
		BERKELIUM_WRITE("berkelium update loop started!\n");
		update();
	} else if(initialised == 2) {
		// everything is ok
	} else {
		fprintf(stderr, "berkelium init error!\n");
	}
}

void Berkelium::destory() {
	fprintf(stderr, "berkelium closed!\n");
	BERKELIUM_WRITE("berkelium closed!\n");
	close(fifo_out);
}

void Berkelium::send(std::string msg) {
	fprintf(stderr, "berkelium send: %s\n", msg.c_str());
	BERKELIUM_WRITE(msg.c_str());
	BERKELIUM_WRITE("\n");
}

bool Berkelium::isActive() {
	return initialised != 0;
}

} // namespace Berkelium
