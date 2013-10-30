// Copyright (c) 2012 The Berkelium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BERKELIUM_TEST_TEST_H_
#define BERKELIUM_TEST_TEST_H_
#pragma once

Berkelium::LoggerRef getTestLogger(std::string clazz, std::string name);

#define DEFINE_LOGGER(clazz)								\
protected:													\
	Berkelium::LoggerRef getLogger(std::string name) {		\
		return getTestLogger(#clazz, name);					\
	}

#define USE_LOGGER(func)									\
	Berkelium::LoggerRef logger(getLogger(#func));

#define ASSERT_NULL(ptr) ASSERT_EQ((void*)NULL, (void*)(ptr.get()))
#define ASSERT_NOT_NULL(ptr) ASSERT_TRUE((void*)(ptr.get()) != NULL)

#define ASSERT_SAME(ptr1, ptr2) ASSERT_EQ((void*)(ptr1.get()), (void*)(ptr2.get()))
#define ASSERT_NOT_SAME(ptr1, ptr2) ASSERT_NE((void*)(ptr1.get()), (void*)(ptr2.get()))

#endif // BERKELIUM_TEST_TEST_H_
