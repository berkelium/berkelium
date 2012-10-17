// Copyright (c) 2012 The Berkelium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "Berkelium.hpp"
#include "WindowActions.hpp"

namespace Berkelium {

WindowActions::WindowActions() {
	Berkelium::addWindow(this);
}

WindowActions::~WindowActions() {
	Berkelium::removeWindow(this);
}

} // namespace Berkelium
