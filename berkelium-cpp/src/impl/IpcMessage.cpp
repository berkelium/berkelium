// Copyright (c) 2012 The Berkelium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "berkelium/IpcMessage.hpp"

#include <cstring>

namespace Berkelium {

namespace impl {

IpcMessage::IpcMessage() {
}

IpcMessage::~IpcMessage() {
}

class IpcMessageImpl : public IpcMessage {
private:
	size_t capacity;
	size_t used;
	int8_t* buffer;

public:
	IpcMessageImpl() :
		capacity(1024),
		used(0),
		buffer(new int8_t[capacity]) {
	}

	virtual ~IpcMessageImpl() {
		capacity = 0;
		used = 0;
		delete buffer;
		buffer = 0;
	}

	virtual size_t length() {
		return 0;
	}

	virtual void* data() {
		return buffer;
	}

	virtual void reset() {
		used = 0;
	}

	virtual void setup(size_t size) {
		if(size > capacity) {
			size += 1024;
			int8_t* tmp = new int8_t[size];
			memcpy(tmp, buffer, used);
			delete buffer;
			buffer = tmp;
			capacity = size;
		}
	}

	void assume(size_t remaining) {
		if(used > remaining) {
			fprintf(stderr, "IpcMessage: buffer underflow! (used:%d remaining:%d capacity:%d)", used, remaining, capacity);
			throw "IpcMessage: buffer underflow!";
		}
	}

	void increase(size_t add) {
		setup(used + add);
	}

	virtual void add_8(int8_t data) {
		increase(1);
		buffer[used++] = data;
	}

	virtual void add_16(int16_t data) {
		increase(2);
		buffer[used++] = (data >> 8) & 0xff;
		buffer[used++] = (data >> 0) & 0xff;
	}

	virtual void add_32(int32_t data) {
		increase(4);
		buffer[used++] = (data >> 24) & 0xff;
		buffer[used++] = (data >> 16) & 0xff;
		buffer[used++] = (data >>  8) & 0xff;
		buffer[used++] = (data >>  0) & 0xff;
	}

	virtual void add32s(int count, const int* data) {
		increase(count * 4);
		add_32(count);
		for(int i = 0; i < count; ++i) {
			add_32(data[i]);
		}
	}

	virtual void add_data16(int16_t size, const void* data) {
		increase(size);
		add_16(size);
		memcpy(&buffer[used], data, size);
		used += size;
	}

	virtual void add_data32(int32_t size, const void* data) {
		increase(size);
		add_32(size);
		memcpy(&buffer[used], data, size);
		used += size;
	}

	virtual void add_str(const char* str) {
		add_data16(strlen(str), str);
	}

	virtual void add_str(const std::string& str) {
		add_data16(str.size(), str.c_str());
	}

	virtual int8_t get_8() {
		assume(1);
		return (int8_t)buffer[used++];
	}

	virtual int16_t get_16() {
		assume(2);
		int16_t ret = 0;
		ret |= buffer[used++];
		ret |= buffer[used++] << 8;
		return ret;
	}

	virtual int32_t get_32() {
		assume(4);
		int32_t ret = 0;
		ret |= buffer[used++];
		ret |= buffer[used++] << 8;
		ret |= buffer[used++] << 16;
		ret |= buffer[used++] << 24;
		return ret;
	}

	virtual Int32Ref get32s() {
		int32_t size = get_32();
		assume(size);
		int32_t* ret = new int32_t[size];
		for(int i = 0; i < size; ++i) {
			ret[i] = get_32();
		}
		return Int32Ref(ret);
	}

	virtual ByteRef get_data16() {
		int16_t size = get_16();
		assume(size);
		int8_t* ret = new int8_t[size];
		for(int i = 0; i < size; ++i) {
			ret[i] = get_8();
		}
		return ByteRef(ret);
	}

	virtual ByteRef get_data32() {
		int32_t size = get_32();
		assume(size);
		int8_t* ret = new int8_t[size];
		for(int i = 0; i < size; ++i) {
			ret[i] = get_8();
		}
		return ByteRef(ret);
	}

	virtual std::string get_str() {
		int16_t size = get_16();
		assume(size);
		fprintf(stderr, "get_str: %d\n", size);
		char* tmp = new char[size];
		for(int i = 0; i < size; ++i) {
			tmp[i] = get_8();
		}
		std::string ret(tmp, size);
		delete[] tmp;
		return ret;
	}
};

IpcMessageRef IpcMessage::create() {
	return IpcMessageRef(new IpcMessageImpl());
}

} // namespace impl

} // namespace Berkelium
