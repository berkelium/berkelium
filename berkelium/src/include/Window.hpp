// Copyright (c) 2012 The Berkelium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef WINDOW_H_
#define WINDOW_H_
#pragma once

namespace Berkelium {

class Window {
public:
	Window();
	virtual ~Window();

	void init();

private:
	Window(const Window&);
	void operator=(const Window&);
};

} // namespace Berkelium

#endif // WINDOW_H_
