/*
//============================================================================
// Name        : berkelium-rpc.cpp
// Author      :
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================



class BerkeliumIpcSend {
public:
	virtual ~BerkeliumIpcSend() {
	}

	virtual void send(size_t len, const void* data) {
	}
};


class BerkeliumRpcBuffer {

private:
	int capacity;
	int used;
	char* buffer;

public:
	BerkeliumRpcBuffer(int initialSize) :
		capacity(initialSize),
		used(0),
		buffer(new char[initialSize]) {
	}

	~BerkeliumRpcBuffer() {
		capacity = 0;
		used = 0;
		delete buffer;
		buffer = 0;
	}

	void reset() {
		used = 0;
	}

	void send(BerkeliumIpcSend* rpc) {
		rpc->send(used, buffer);
		reset();
	}

	void increase(int add) {
		int size = used + add;
		if(size > capacity) {
			size += 1024;
			char* tmp = new char[size];
			memcpy(tmp, buffer, used);
			delete buffer;
			buffer = tmp;
		}
	}

	void add_8(char data) {
		increase(1);
		buffer[used++] = data;
	}

	void add_16(short data) {
		increase(2);
		buffer[used++] = (data >> 8) & 0xff;
		buffer[used++] = (data >> 0) & 0xff;
	}

	void add_32(int data) {
		increase(4);
		buffer[used++] = (data >> 24) & 0xff;
		buffer[used++] = (data >> 16) & 0xff;
		buffer[used++] = (data >>  8) & 0xff;
		buffer[used++] = (data >>  0) & 0xff;
	}

	void add32s(int count, const int* data) {
		increase((count+1) * 4);
		add_32(count);
		for(int i = 0; i < count; ++i) {
			add_32(data[i]);
		}
	}

	void add_data(int size, const void* data) {
		increase(size + 4);
		add_32(size);
		memcpy(&buffer[used], data, size);
		used += size;
	}

	void add_str(const char* data) {
		add_data(strlen(data), data);
	}
};

int main(int argc, const char** argv) {
	if (argc != 3) {
		std::cerr << "Syntax: " << argv[0] << " port key" << std::endl;
		return 0;
	}
	IpcSocket rpc;
	if (!rpc.init()) {
		std::cerr << "init failed" << std::endl;
		return 0;
	}

	if (!rpc.connect("127.0.0.1", argv[1])) {
		std::cerr << "connect failed" << std::endl;
		return 0;
	}

	BerkeliumRpcBuffer buf(100);

	buf.add_32(0);
	buf.add_str(argv[2]);
	buf.send(&rpc);

	buf.add_32(1);
	buf.add_8(88);
	buf.add_str("helloworld");
	buf.add_8(99);
	buf.send(&rpc);

	buf.add_32(2);
	buf.add_8(11);
	buf.add_str("abcdef");
	buf.add_8(22);
	buf.send(&rpc);

	buf.add_32(3);
	buf.add_str("0123456789abcdef");
	buf.send(&rpc);

	buf.add_32(4);
	buf.add_8(10);
	buf.add_8(20);
	buf.add_8(30);
	buf.add_16(0xabcde);
	buf.add_8(40);
	buf.add_32(0x12345678);
	buf.add_8(50);
	buf.send(&rpc);

	buf.add_32(5);
	buf.send(&rpc);

	buf.add_32(6);
	buf.add_16(6);
	buf.send(&rpc);

	buf.add_32(7);
	buf.add_32(7);
	buf.send(&rpc);

	int ints[] = {
		11,22,33,44,55,66,77,88,99
	};
	buf.add_32(8);
	buf.add32s(9, ints);
	buf.send(&rpc);

	rpc.close();

	return 1;
}

*/
