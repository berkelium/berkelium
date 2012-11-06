// Copyright (c) 2012 The Berkelium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BERKELIUM_PIPE_HPP_
#define BERKELIUM_PIPE_HPP_
#pragma once

#ifndef BERKELIUM_CPP_IMPL
# error "This file is intended for internal use only!"
#endif

#include "berkelium/Impl.hpp"

namespace Berkelium {

namespace impl {

class Pipe {
protected:
	Pipe();

public:
	static PipeRef getPipe(const std::string& path);

	virtual ~Pipe() = 0;

	// Returns true if there are no pending messages to receive.
	virtual bool isEmpty() = 0;

	// Sends this message.
	virtual void send(const std::string& msg) = 0;

	// Receives the next message.
	virtual const std::string recv() = 0;

	virtual const std::string getPath() = 0;
};

} // namespace impl

} // namespace Berkelium

#endif // BERKELIUM_PIPE_HPP_
