// Copyright (c) 2012 The Berkelium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BERKELIUM_IMPL_HPP_
#define BERKELIUM_IMPL_HPP_
#pragma once

#ifndef BERKELIUM_CPP_IMPL
# error "This file is intended for internal use only!"
#endif

#include "berkelium/Berkelium.hpp"

namespace Berkelium {

namespace impl {

HostVersionRef newHostVersion(int32_t major, int32_t minor, int32_t build, int32_t patch);
HostExecutableRef newHostExecutable();
HostExecutableRef newHostExecutable(const std::string&);
ProfileRef newProfile();

} // namespace impl

} // namespace Berkelium

#endif // BERKELIUM_IMPL_HPP_
