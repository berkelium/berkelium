// Copyright (c) 2012 The Berkelium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Berkelium/API/LogDelegate.hpp>
#include <Berkelium/API/Logger.hpp>
#include <Berkelium/IPC/Message.hpp>
#include <cstring>

namespace Berkelium {

namespace Ipc {

namespace impl {

class MessageImpl : public Message {
private:
	LoggerRef logger;
	size_t capacity;
	size_t wp;
	size_t rp;
	uint8_t* real;
	uint8_t* buffer;
	int32_t* id;

public:
	MessageImpl(LoggerRef logger) :
		Message(),
		logger(logger),
		capacity(1024),
		wp(0),
		rp(0),
		real(new uint8_t[capacity + sizeof(int32_t)]),
		buffer(real + sizeof(int32_t)),
		id((int32_t*)real) {
	}

	virtual ~MessageImpl() {
		capacity = 0;
		wp = 0;
		rp = 0;
		delete[] real;
		real = 0;
		buffer = 0;
		id = 0;
	}

	virtual int32_t getChannelId() {
		return *id;
	}

	virtual void setChannelId(int32_t id) {
		*(this->id) = id;
	}

	virtual size_t length() {
		return wp;
	}

	virtual size_t remaining() {
		return wp - rp;
	}

	virtual void* data() {
		return real;
	}

	virtual size_t data_length() {
		return wp + sizeof(int32_t);
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
			uint8_t* tmp = new uint8_t[size + sizeof(int32_t)];
			memcpy(tmp, real, wp + sizeof(int32_t));
			delete[] real;
			real = tmp;
			buffer = tmp + 4;
			id = (int32_t*)tmp;
			capacity = size;
		}
	}

	void assume(size_t remaining) {
		if(rp + remaining > wp) {
			logger->error() << "IpcMessage: buffer underflow! (wp:" << wp << " rp:" << remaining << " capacity:" << capacity << ")" << std::endl;
			throw "IpcMessage: buffer underflow!";
		}
	}

	void increase(size_t add) {
		resize(wp + add);
	}

	virtual void add_cmd(CommandId id) {
		add_32((CommandId)id);
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

	virtual CommandId get_cmd() {
		return (CommandId)get_32();
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

#if 0
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
#endif

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

} // namespace impl

Message::Message() {
}

Message::~Message() {
}

MessageRef Message::create(LoggerRef logger) {
	return MessageRef(new impl::MessageImpl(logger));
}

} // namespace Ipc

} // namespace Berkelium
