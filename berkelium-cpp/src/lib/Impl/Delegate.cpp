// Copyright (c) 2013 The Berkelium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Berkelium/API/HostDelegate.hpp>
#include <Berkelium/API/TabDelegate.hpp>
#include <Berkelium/Impl/Impl.hpp>

namespace Berkelium {

HostDelegate::HostDelegate() {
	TRACE_OBJECT_NEW("HostDelegate");
}

HostDelegate::~HostDelegate() {
	TRACE_OBJECT_DELETE("HostDelegate");
}

TabDelegate::TabDelegate() {
	TRACE_OBJECT_NEW("TabDelegate");
}

TabDelegate::~TabDelegate() {
	TRACE_OBJECT_DELETE("TabDelegate");
}

} // namespace Berkelium
