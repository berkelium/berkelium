// Copyright (c) 2012 The Berkelium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BERKELIUM_PROCESS_HPP_
#define BERKELIUM_PROCESS_HPP_
#pragma once

#include "berkelium/Berkelium.hpp"
#include "berkelium/Ipc.hpp"

#ifndef BERKELIUM_CPP_IMPL
# error "This file is intended for internal use only!"
#endif

namespace Berkelium {

namespace impl {

class Process;
typedef std::shared_ptr<Process> ProcessRef;

class Process {
private:
	IpcRef ipc;

protected:
	inline Process(const std::string& dir) :
		ipc(Ipc::getIpc(dir, true)) {
	}

public:
	static ProcessRef create(const std::string& dir);

	virtual ~Process() = 0;

	IpcRef getIpc() {
		return ipc;
	}

	virtual const bool start(const std::vector<std::string>& args) = 0;
};

} // namespace impl

} // namespace Berkelium

#endif // BERKELIUM_PROCESS_HPP_
