// Copyright (c) 2012 The Berkelium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BERKELIUM_IPC_MESSAGE_HPP_
#define BERKELIUM_IPC_MESSAGE_HPP_
#pragma once

#ifndef BERKELIUM_CPP_IMPL
# error "This file is intended for internal use only!"
#endif

#include "berkelium/Impl.hpp"

namespace Berkelium {

namespace impl {

class IpcMessage {
protected:
	IpcMessage();

public:
	static IpcMessageRef create();

	virtual ~IpcMessage() = 0;

	virtual size_t length() = 0;
	virtual void* data() = 0;
	virtual void reset() = 0;
	virtual void setup(size_t) = 0;

	virtual void add_8(int8_t) = 0;
	virtual void add_16(int16_t) = 0;
	virtual void add_32(int32_t) = 0;
	virtual void add32s(int32_t, const int* data) = 0;
	virtual void add_data16(int16_t, const void* data) = 0;
	virtual void add_data32(int32_t, const void* data) = 0;
	virtual void add_str(const char*) = 0;
	virtual void add_str(const std::string&) = 0;

	virtual int8_t get_8() = 0;
	virtual int16_t get_16() = 0;
	virtual int32_t get_32() = 0;
	virtual Int32Ref get32s() = 0;
	virtual ByteRef get_data16() = 0;
	virtual ByteRef get_data32() = 0;
	virtual std::string get_str() = 0;
};

} // namespace impl

} // namespace Berkelium

#endif // BERKELIUM_IPC_MESSAGE_HPP_
