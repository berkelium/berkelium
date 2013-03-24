// Copyright (c) 2013 The Berkelium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BERKELIUM_API_LOGGER_HPP_
#define BERKELIUM_API_LOGGER_HPP_
#pragma once

#ifndef BERKELIUM_CPP_IMPL
# error "This file is intended for internal use only!"
#endif

#include <iostream>

namespace Berkelium {

namespace Log {

void setPrefix(const std::string& prefix);

std::ostream& debug();
std::ostream& info();
std::ostream& warn();
std::ostream& error();

void systemError(const std::string& error);

} // namespace Log

} // namespace Berkelium

#endif // BERKELIUM_IMPL_LOGGER_HPP_
