// Copyright (c) 2012 The Berkelium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "../include/PacketWriter.hpp"
#include "../include/IpcSender.hpp"
//#include "../include/Rect.hpp"
#include <cstring>

namespace Berkelium {

PacketWriter::PacketWriter(IpcSender* sender, int initialSize) :
	sender(sender),
	capacity(initialSize),
	used(0),
	buffer(new char[initialSize]) {
}

PacketWriter::~PacketWriter() {
	capacity = 0;
	used = 0;
	delete buffer;
	buffer = 0;
}

void PacketWriter::reset() {
	used = 0;
}

void PacketWriter::send() {
	sender->send(used, buffer);
	reset();
}

void PacketWriter::increase(int add) {
	int size = used + add;
	if(size > capacity) {
		size += 1024;
		char* tmp = new char[size];
		memcpy(tmp, buffer, used);
		delete buffer;
		buffer = tmp;
	}
}

void PacketWriter::add_8(char data) {
	increase(1);
	buffer[used++] = data;
}

void PacketWriter::add_16(short data) {
	increase(2);
	buffer[used++] = (data >> 8) & 0xff;
	buffer[used++] = (data >> 0) & 0xff;
}

void PacketWriter::add_32(int data) {
	increase(4);
	buffer[used++] = (data >> 24) & 0xff;
	buffer[used++] = (data >> 16) & 0xff;
	buffer[used++] = (data >>  8) & 0xff;
	buffer[used++] = (data >>  0) & 0xff;
}

void PacketWriter::add32s(int count, const int* data) {
	increase((count+1) * 4);
	add_32(count);
	for(int i = 0; i < count; ++i) {
		add_32(data[i]);
	}
}

void PacketWriter::add_data(int size, const void* data) {
	increase(size + 4);
	add_32(size);
	memcpy(&buffer[used], data, size);
	used += size;
}

void PacketWriter::add_str(const char* data) {
	add_data(strlen(data), data);
}

void PacketWriter::add_str(const std::string& data) {
	add_data(data.size(), data.c_str());
}


} // namespace Berkelium
