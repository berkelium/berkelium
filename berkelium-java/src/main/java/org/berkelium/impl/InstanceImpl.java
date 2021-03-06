// Copyright (c) 2013 The Berkelium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

package org.berkelium.impl;

import org.berkelium.api.HostDelegate;
import org.berkelium.api.HostExecutable;
import org.berkelium.api.Instance;
import org.berkelium.api.Profile;
import org.berkelium.api.Runtime;
import org.berkelium.api.Window;

// =========================================
// WARNING:
//   THIS FILE IS AUTOMATICALLY GENERATED!
//   !! ANY CHANGES WILL BE OVERWRITTEN !!
//
// See berkelium/berkelium-api/update.sh
// =========================================

class InstanceImpl implements Instance, BerkeliumObjectImpl {

	@Override
	public native void dispose();

	@Override
	public native Runtime getRuntime();

	@Override
	public native void close();

	@Override
	public native Profile getProfile();

	@Override
	public native HostExecutable getExecutable();

	@Override
	public native void addHostDelegate(HostDelegate delegate);

	@Override
	public native void removeHostDelegate(HostDelegate delegate);

	@Override
	public native int getWindowCount();

	@Override
	public native java.util.List<Window> getWindowList();

	@Override
	public native Window createWindow(boolean incognito);
}
