// Copyright (c) 2012 The Berkelium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef WINDOWACTIONS_HPP_
#define WINDOWACTIONS_HPP_

namespace Berkelium {

class WindowActions {
public:
	WindowActions();
	virtual ~WindowActions() = 0;

	virtual void Shutdown() = 0;

private:
	WindowActions(const WindowActions&);
	void operator=(const WindowActions&);
};

} // namespace Berkelium

#endif /* WINDOWACTIONS_HPP_ */
