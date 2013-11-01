// Copyright (c) 2013 The Berkelium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BERKELIUM_IMPL_UPDATE_QUEUE_HPP_
#define BERKELIUM_IMPL_UPDATE_QUEUE_HPP_
#pragma once

#include <Berkelium/Impl/Impl.hpp>

namespace Berkelium {

namespace impl {

class UpdateQueue {
private:
	Ipc::LinkGroupRef group;
	UpdateRefMap updates;

public:
	UpdateQueue(Ipc::LinkGroupRef group);

	virtual ~UpdateQueue();

	virtual void addUpdateEvent(UpdateRef update, int32_t timeout);

	virtual void removeUpdateEvent(UpdateRef update);

	virtual void update(int32_t timeout);
};

} // namespace impl

} // namespace Berkelium

#endif // BERKELIUM_IMPL_UPDATE_QUEUE_HPP_
