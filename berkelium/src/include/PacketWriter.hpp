// Copyright (c) 2012 The Berkelium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef PACKET_WRITER_HPP_
#define PACKET_WRITER_HPP_
#pragma once

#include <string>

namespace Berkelium {

class IpcSender;

class PacketWriter {
private:
	IpcSender* sender;
	int capacity;
	int used;
	char* buffer;

public:
	PacketWriter(IpcSender*, int initialSize);
	~PacketWriter();

	void reset();
	void send();
	void increase(int);
	void add_8(char);
	void add_16(short);
	void add_32(int);
	void add32s(int, const int* data);
	void add_data(int, const void* data);
	void add_str(const char*);
	void add_str(const std::string&);

};

} // namespace Berkelium


#endif // PACKET_WRITER_HPP_
