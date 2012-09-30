// Copyright (c) 2012 The Berkelium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef IPC_SENDER_HPP_
#define IPC_SENDER_HPP_
#pragma once

namespace Berkelium {

class IpcSender {
public:
	virtual ~IpcSender();
	virtual void send(int len, const void* data);
};

} // namespace Berkelium

#endif // IPC_SENDER_HPP_
