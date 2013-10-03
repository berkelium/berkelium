// Copyright (c) 2013 The Berkelium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BERKELIUM_HOST_INSTANCE_HPP_
#define BERKELIUM_HOST_INSTANCE_HPP_
#pragma once

#include <Berkelium/IPC/Pipe.hpp>

namespace Berkelium {

class BerkeliumHostInstance;
typedef std::shared_ptr<BerkeliumHostInstance> BerkeliumHostInstanceRef;
typedef std::weak_ptr<BerkeliumHostInstance> BerkeliumHostInstanceWRef;

class BerkeliumHostInstance : public Berkelium::Ipc::PipeCallback {
protected:
	BerkeliumHostInstance();

public:
	static BerkeliumHostInstanceRef createBerkeliumHostInstance(LoggerRef logger, Ipc::PipeGroupRef group, Ipc::ChannelRef ipc);

	virtual ~BerkeliumHostInstance() = 0;
};

} // namespace Berkelium

#endif // BERKELIUM_HOST_INSTANCE_HPP_
