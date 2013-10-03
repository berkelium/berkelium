// Copyright (c) 2012 The Berkelium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BERKELIUM_IPC_PIPE_HPP_
#define BERKELIUM_IPC_PIPE_HPP_
#pragma once

#ifndef BERKELIUM_CPP_IMPL
# error "This file is intended for internal use only!"
#endif

#include <Berkelium/IPC/Ipc.hpp>

namespace Berkelium {

namespace Ipc {

class PipeCallback
{
public:
	PipeCallback();

	virtual ~PipeCallback() = 0;

	virtual void onDataReady(PipeRef pipe) = 0;
};

class Pipe {
protected:
	Pipe();

public:
	static PipeRef getPipe(bool read, PipeGroupRef group, LoggerRef logger, const std::string& dir, const std::string& name, const std::string& alias);

	virtual ~Pipe() = 0;

	// Returns true if there are no pending messages to receive.
	virtual bool isEmpty() = 0;

	// Sends this message.
	virtual void send(MessageRef msg) = 0;

	// Receives the next message.
	virtual void recv(MessageRef msg) = 0;

	virtual const std::string getName() = 0;

	virtual const std::string getAlias() = 0;
};

} // namespace Ipc

} // namespace Berkelium

#endif // BERKELIUM_IPC_PIPE_HPP_
