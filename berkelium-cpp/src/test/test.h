// Copyright (c) 2012 The Berkelium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BERKELIUM_TEST_TEST_H_
#define BERKELIUM_TEST_TEST_H_
#pragma once

#define ASSERT_NULL(ptr) ASSERT_EQ((void*)NULL, (void*)(ptr.get()))
#define ASSERT_NOT_NULL(ptr) ASSERT_NE((void*)NULL, (void*)(ptr.get()))

#endif // BERKELIUM_TEST_TEST_H_
