// Copyright (c) 2013 The Berkelium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BERKELIUM_IPC_PIPE_POSIX_HPP_
#define BERKELIUM_IPC_PIPE_POSIX_HPP_
#pragma once

#ifdef LINUX

#ifndef BERKELIUM_CPP_IMPL
# error "This file is intended for internal use only!"
#endif

#include <Berkelium/IPC/Link.hpp>

namespace Berkelium {

namespace Ipc {

// same api as link...
class PipePosix : public Link {
protected:
	PipePosix();

public:
	virtual ~PipePosix() = 0;

	static PipePosixRef create(bool read, LoggerRef logger, const std::string& dir, const std::string& name, const std::string& alias);
};

} // namespace Ipc

} // namespace Berkelium

#endif // LINUX

#endif // BERKELIUM_IPC_PIPE_POSIX_HPP_
