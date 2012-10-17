// Copyright (c) 2012 The Berkelium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef IPC_SOCKET_HPP_
#define IPC_SOCKET_HPP_
#pragma once

#include "IpcSender.hpp"

namespace Berkelium {

class IpcSocket : public IpcSender {

private:
	int sock;

public:
	IpcSocket();
	virtual ~IpcSocket();

	int init();

	int connect(const char* host, const char* port);

	int connect(const char* host, int port);

	void close();

	void send(int op, int len, const void* data);

	void send(int len, const void* data);

	bool recv();
};

} // namespace Berkelium

#endif // IPC_SOCKET_HPP_
