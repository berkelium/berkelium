// Copyright (c) 2012 The Berkelium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "WindowDelegate.hpp"

#include <stdio.h>

namespace Berkelium {

WindowDelegate::WindowDelegate() {
}

WindowDelegate::~WindowDelegate() {
}

void WindowDelegate::onPaint(Window *win, const unsigned char *sourceBuffer, const Rect &sourceBufferRect, size_t numCopyRects, const Rect *copyRects, int dx, int dy, const Rect &scrollRect) {
	fprintf(stderr, "WindowDelegate::onPaint\n");
}

void WindowDelegate::onTitleChanged(Window *win, std::string title) {
	fprintf(stderr, "WindowDelegate::onTitleChanged '%s'\n", title.c_str());
}

} // namespace Berkelium
