// Copyright (c) 2013 The Berkelium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Berkelium/Impl/Impl.hpp>

#include <stdio.h>
#include <stdarg.h>

namespace Berkelium {

namespace impl {

#define BUF_SIZE 4096

void bk_error(const char* format, ...)
{
	char buffer[BUF_SIZE];
	va_list va;
	va_start(va, format);
	vsnprintf(buffer, BUF_SIZE, format, va);
	va_end(va);
	fprintf(stderr, "bk_error: %s\n", buffer);
}

} // namespace impl

} // namespace Berkelium
