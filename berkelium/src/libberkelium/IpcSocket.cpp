// Copyright (c) 2012 The Berkelium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "../include/IpcSocket.hpp"
#include <iostream>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>

#ifdef _WIN32
#include <winsock.h>
#include <io.h>
#else
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#endif

namespace Berkelium {

IpcSocket::IpcSocket() :
	sock(-1) {
}

IpcSocket::~IpcSocket() {
	close();
}

int IpcSocket::init() {
#ifdef _WIN32
	short wVersionRequested = MAKEWORD (1, 1);
	WSADATA wsaData;
	if (WSAStartup(wVersionRequested, &wsaData) != 0) {
		return 0;
	}
#endif
	return 1;
}

int IpcSocket::connect(const char* host, const char* port) {
	return connect(host, atoi(port));
}

int IpcSocket::connect(const char* host, int port) {
	int sock = socket(PF_INET, SOCK_STREAM, 0);
	if (sock < 0) {
		return -1;
	}
	struct sockaddr_in server;
	unsigned long addr;

	memset(&server, 0, sizeof(server));
	if ((addr = inet_addr(host)) == INADDR_NONE) {
		return -2;
	}
	memcpy((char *) &server.sin_addr, &addr, sizeof(addr));

	server.sin_family = AF_INET;
	server.sin_port = htons(port);

	if (::connect(sock, (struct sockaddr*) &server, sizeof(server)) < 0) {
		return -3;
	}

	this->sock = sock;
	return 1;
}

void IpcSocket::close() {
	if (sock != -1) {
		int tmp = 0;
		::send(sock, (const char*) &tmp, 4, 0);
		::close(sock);
		sock = -1;
	}
}

void IpcSocket::send(int op, int len, const void* data) {
	int tmp = htonl(len+4);
	::send(sock, (const char*) &tmp, 4, 0 );
	tmp = htonl(op);
	::send(sock, (const char*) &tmp, 4, 0 );
	::send(sock, (const char*) data, len, 0);
}

void IpcSocket::send(int len, const void* data) {
	int tmp = htonl(len);
	::send(sock, (const char*) &tmp, 4, 0 );
	::send(sock, (const char*) data, len, 0);
}

void IpcSocket::recv() {
	fd_set rfds;
	FD_ZERO(&rfds);
	FD_SET(sock, &rfds);

	struct timeval tv;
	tv.tv_sec = 0;
	tv.tv_usec = 0;
	int retval = select(sock + 1, &rfds, NULL, NULL, &tv);
	if (retval == -1) {
		perror("select()");
	} else if(retval) {
		char data[1024];
		size_t size = read(sock, &data, 1024);
		write(sock, data, size);
		fprintf(stderr, "DATA\n");
	}
}

} // namespace Berkelium
