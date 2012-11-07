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
	size_t wp;
	size_t rp;
	int8_t* buffer;

public:
	IpcMessageImpl() :
		capacity(1024),
		wp(0),
		rp(0),
		buffer(new int8_t[capacity]) {
	}

	virtual ~IpcMessageImpl() {
		capacity = 0;
		wp = 0;
		rp = 0;
		delete buffer;
		buffer = 0;
	}

	virtual size_t length() {
		return wp;
	}

	virtual size_t remaining() {
		return wp - rp;
	}

	virtual void* data() {
		return buffer;
	}

	virtual void reset() {
		wp = 0;
		rp = 0;
	}

	virtual void setup(size_t size) {
		resize(size);
		rp = 0;
		wp = size;
	}

	void resize(size_t size) {
		if(size > capacity) {
			size += 1024;
			int8_t* tmp = new int8_t[size];
			memcpy(tmp, buffer, wp);
			delete buffer;
			buffer = tmp;
			capacity = size;
		}
	}

	void assume(size_t remaining) {
		if(rp + remaining > wp) {
			fprintf(stderr, "IpcMessage: buffer underflow! (wp:%lud rp:%lud capacity:%lud)", wp, remaining, capacity);
			throw "IpcMessage: buffer underflow!";
		}
	}

	void increase(size_t add) {
		resize(wp + add);
	}

	virtual void add_8(int8_t data) {
		increase(1);
		buffer[wp++] = data;
	}

	virtual void add_16(int16_t data) {
		increase(2);
		buffer[wp++] = (data >> 8) & 0xff;
		buffer[wp++] = (data >> 0) & 0xff;
	}

	virtual void add_32(int32_t data) {
		increase(4);
		buffer[wp++] = (data >> 24) & 0xff;
		buffer[wp++] = (data >> 16) & 0xff;
		buffer[wp++] = (data >>  8) & 0xff;
		buffer[wp++] = (data >>  0) & 0xff;
	}

	virtual void add32s(int count, const int* data) {
		increase(count * 4 + 4);
		add_32(count);
		for(int i = 0; i < count; ++i) {
			add_32(data[i]);
		}
	}

	virtual void add_data16(int16_t size, const void* data) {
		increase(size + 2);
		add_16(size);
		memcpy(&buffer[wp], data, size);
		wp += size;
	}

	virtual void add_data32(int32_t size, const void* data) {
		increase(size);
		add_32(size);
		memcpy(&buffer[wp], data, size);
		wp += size;
	}

	virtual void add_str(const char* str) {
		add_data16(strlen(str), str);
	}

	virtual void add_str(const std::string& str) {
		add_data16(str.size(), str.c_str());
	}

	virtual int8_t get_8() {
		assume(1);
		return (int8_t)buffer[rp++];
	}

	virtual int16_t get_16() {
		assume(2);
		int16_t ret = 0;
		ret |= buffer[rp++] << 8;
		ret |= buffer[rp++];
		return ret;
	}

	virtual int32_t get_32() {
		assume(4);
		int32_t ret = 0;
		ret |= buffer[rp++] << 24;
		ret |= buffer[rp++] << 16;
		ret |= buffer[rp++] << 8;
		ret |= buffer[rp++];
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
