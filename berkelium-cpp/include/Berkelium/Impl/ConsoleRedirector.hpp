// Copyright (c) 2013 The Berkelium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BERKELIUM_IMPL_CONSOLE_REDIRECTOR_HPP_
#define BERKELIUM_IMPL_CONSOLE_REDIRECTOR_HPP_
#pragma once

#ifndef BERKELIUM_CPP_IMPL
# error "This file is intended for internal use only!"
#endif

namespace Berkelium {

namespace impl {

class ConsoleRedirector
{
private:
	RuntimeRef runtime;
	LogType type;
	std::string buffer;

public:
	ConsoleRedirector(RuntimeRef runtime, LogType type);

	~ConsoleRedirector();

	void update(std::string& buffer);

	void log(std::string line);
};

} // namespace impl

} // namespace Berkelium

#endif // BERKELIUM_IMPL_CONSOLE_REDIRECTOR_HPP_
