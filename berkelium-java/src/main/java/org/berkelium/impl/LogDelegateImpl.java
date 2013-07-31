// Copyright (c) 2013 The Berkelium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

package org.berkelium.impl;

import org.berkelium.api.LogDelegate;
import org.berkelium.api.LogSource;
import org.berkelium.api.LogType;
import org.berkelium.api.Runtime;

// =========================================
// WARNING:
//   THIS FILE IS AUTOMATICALLY GENERATED!
//   !! ANY CHANGES WILL BE OVERWRITTEN !!
//
// See berkelium/berkelium-api/update.sh
// =========================================

public class LogDelegateImpl implements LogDelegate {

	@Override
	public native void log(Runtime runtime, LogSource source, LogType type, String clazz, String name, String message);
}
