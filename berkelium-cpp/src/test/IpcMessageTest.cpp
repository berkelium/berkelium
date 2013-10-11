// Copyright (c) 2012 The Berkelium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Berkelium/IPC/Message.hpp>

#include "gtest/gtest.h"
#include "test.h"

using Berkelium::Ipc::Message;
using Berkelium::Ipc::MessageRef;

namespace {

class IpcMessageTest : public ::testing::Test {
	DEFINE_LOGGER(IpcMessageTest);
};

TEST_F(IpcMessageTest, create) {
	USE_LOGGER(create);
	ASSERT_NOT_NULL(Message::create(logger));
}

/*
inline void dump(char* data, size_t size) {
	std::stringstream str;
	str << "dump(" << size << "): ";
	str << std::hex;
	for(size_t i = 0; i < size; i++) {
		str << (int)(data[i] & 0xFF);
		str << ' ';
	}
	str << ' ';
	for(size_t i = 0; i < size; i++) {
		str << (char)(data[i] >= 32 ? data[i] : '.');
	}
	fprintf(stderr, "%s\n", str.str().c_str());
}
*/

#define TEST_LR(l, r) ASSERT_EQ((size_t)l, m->length()); ASSERT_EQ((size_t)r, m->remaining())

TEST_F(IpcMessageTest, test8) {
	USE_LOGGER(test8);
	MessageRef m = Message::create(logger);
	TEST_LR(0, 0);

	m->add_8(   0);TEST_LR(1, 1);
	m->add_8( 100);TEST_LR(2, 2);
	m->add_8(  -1);TEST_LR(3, 3);
	m->add_8(-100);TEST_LR(4, 4);

	ASSERT_EQ(   0, m->get_8());TEST_LR(4, 3);
	ASSERT_EQ( 100, m->get_8());TEST_LR(4, 2);
	ASSERT_EQ(  -1, m->get_8());TEST_LR(4, 1);
	ASSERT_EQ(-100, m->get_8());TEST_LR(4, 0);

	m->reset();TEST_LR(0, 0);
}

TEST_F(IpcMessageTest, test16) {
	USE_LOGGER(test16);
	MessageRef m = Message::create(logger);
	TEST_LR(0, 0);

	m->add_16(   0);TEST_LR(2, 2);
	m->add_16( 100);TEST_LR(4, 4);
	m->add_16(  -1);TEST_LR(6, 6);
	m->add_16(-100);TEST_LR(8, 8);

	ASSERT_EQ(   0, m->get_16());TEST_LR(8, 6);
	ASSERT_EQ( 100, m->get_16());TEST_LR(8, 4);
	ASSERT_EQ(  -1, m->get_16());TEST_LR(8, 2);
	ASSERT_EQ(-100, m->get_16());TEST_LR(8, 0);

	m->reset();TEST_LR(0, 0);
}

TEST_F(IpcMessageTest, uint) {
	USE_LOGGER(uint);
	MessageRef m = Message::create(logger);
	m->add_32(1234567890);
	ASSERT_EQ(1234567890, m->get_32());
}

TEST_F(IpcMessageTest, test32) {
	USE_LOGGER(test32);
	MessageRef m = Message::create(logger);
	TEST_LR(0, 0);

	m->add_32(   0);TEST_LR(4, 4);
	m->add_32( 100);TEST_LR(8, 8);
	m->add_32(  -1);TEST_LR(12, 12);
	m->add_32(-100);TEST_LR(16, 16);

	ASSERT_EQ(   0, m->get_32());TEST_LR(16, 12);
	ASSERT_EQ( 100, m->get_32());TEST_LR(16, 8);
	ASSERT_EQ(  -1, m->get_32());TEST_LR(16, 4);
	ASSERT_EQ(-100, m->get_32());TEST_LR(16, 0);

	m->reset();TEST_LR(0, 0);
}

TEST_F(IpcMessageTest, test_c_str) {
	USE_LOGGER(test_c_str);
	MessageRef m = Message::create(logger);
	TEST_LR(0, 0);

	const char* TEST_STR = "test";
	size_t TEST_SIZE = 6; // (2 byte length) + str

	m->add_str(TEST_STR);
	TEST_LR(TEST_SIZE, TEST_SIZE);
	std::string str = m->get_str();
	TEST_LR(TEST_SIZE, 0);

	ASSERT_STREQ(TEST_STR, str.c_str());

	m->reset();TEST_LR(0, 0);
}

TEST_F(IpcMessageTest, test_std_str) {
	USE_LOGGER(test_std_str);
	MessageRef m = Message::create(logger);
	TEST_LR(0, 0);

	std::string TEST_STR = "test";
	size_t TEST_SIZE = 6; // (2 byte length) + str

	m->add_str(TEST_STR);
	TEST_LR(TEST_SIZE, TEST_SIZE);
	std::string str = m->get_str();
	TEST_LR(TEST_SIZE, 0);

	ASSERT_STREQ(TEST_STR.c_str(), str.c_str());

	m->reset();TEST_LR(0, 0);
}

TEST_F(IpcMessageTest, test_setup) {
	USE_LOGGER(test_setup);
	MessageRef m = Message::create(logger);
	TEST_LR(0, 0);

	char data[] = {0, 0, 0, 0,/* ChannelId */
			0, 4, /* String Length */
			't', 'e', 's', 't'};
	const size_t s(sizeof(data) - sizeof(int32_t));
	m->setup(s);
	::memcpy(m->data(), data, s + sizeof(int32_t));

	TEST_LR(s, s);
	std::string str = m->get_str();
	TEST_LR(s, 0);

	ASSERT_STREQ("test", str.c_str());

	m->reset();TEST_LR(0, 0);
}

TEST_F(IpcMessageTest, test_sendrecv) {
	USE_LOGGER(test_setup);
	MessageRef m = Message::create(logger);
	TEST_LR(0, 0);
	//dump((char*)m->data(), m->data_length());
	m->add_32(   0);TEST_LR(4, 4);
	m->add_32( 100);TEST_LR(8, 8);
	m->add_32(  -1);TEST_LR(12, 12);
	m->add_32(-100);TEST_LR(16, 16);

	MessageRef m2 = Message::create(logger);
	m2->setup(m->data_length() - sizeof(int32_t));
	::memcpy(m2->data(), m->data(), m->data_length());

	// TEST_LR will use m...
	m = m2;
	ASSERT_EQ(   0, m->get_32());TEST_LR(16, 12);
	ASSERT_EQ( 100, m->get_32());TEST_LR(16, 8);
	ASSERT_EQ(  -1, m->get_32());TEST_LR(16, 4);
	ASSERT_EQ(-100, m->get_32());TEST_LR(16, 0);
}

} // namespace
