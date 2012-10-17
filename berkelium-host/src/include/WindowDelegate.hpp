// Copyright (c) 2012 The Berkelium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef WINDOW_DELEGATE_H_
#define WINDOW_DELEGATE_H_
#pragma once

#include "Window.hpp"
#include "Rect.hpp"

#include <string>

namespace Berkelium {

class WindowDelegate {
public:
	WindowDelegate();
	virtual ~WindowDelegate();

	virtual void onPaint(Window *win, const unsigned char *sourceBuffer, const Rect &sourceBufferRect, size_t numCopyRects, const Rect *copyRects, int dx, int dy, const Rect &scrollRect);
	virtual void onTitleChanged(Window *win, std::string title);

private:
	WindowDelegate(const WindowDelegate&);
	void operator=(const WindowDelegate&);
};

} // namespace Berkelium

#endif // WINDOW_DELEGATE_H_
