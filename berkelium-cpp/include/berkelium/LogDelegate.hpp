// Copyright (c) 2012 The Berkelium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BERKELIUM_LOGDELEGATE_HPP_
#define BERKELIUM_LOGDELEGATE_HPP_
#pragma once

// =========================================
// WARNING:
//   THIS FILE IS AUTOMATICALLY GENERATED!
//   !! ANY CHANGES WILL BE OVERWRITTEN !!
//
// See berkelium/berkelium-api/update.sh
// =========================================

#include "berkelium/Berkelium.hpp"

namespace Berkelium {

// Handler for log messages. LogAdapter is an abstract class with empty function definitions for all functions in LogDelegate.
class LogDelegate {
protected:
	LogDelegate();

public:
	virtual ~LogDelegate() = 0;

	// Allows the client application to handle berkelium host console messages.
	virtual void log(InstanceRef instance, LogType type, const std::string& message) = 0;
};

} // namespace Berkelium

#endif // BERKELIUM_LOGDELEGATE_HPP_
