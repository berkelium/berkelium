// Copyright (c) 2012 The Berkelium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "Window.hpp"

#include <stdio.h>

namespace Berkelium {

Window::Window() {
	fprintf(stderr, "new Window\n");
}

Window::~Window() {
	fprintf(stderr, "delete Window\n");
}

void Window::init() {
	fprintf(stderr, "Window::init\n");
}

} // namespace Berkelium
