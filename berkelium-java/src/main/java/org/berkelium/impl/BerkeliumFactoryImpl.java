// Copyright (c) 2013 The Berkelium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

package org.berkelium.impl;

import org.berkelium.api.BerkeliumFactory;
import org.berkelium.api.Runtime;

// =========================================
// WARNING:
//   THIS FILE IS AUTOMATICALLY GENERATED!
//   !! ANY CHANGES WILL BE OVERWRITTEN !!
//
// See berkelium/berkelium-api/update.sh
// =========================================

public class BerkeliumFactoryImpl extends BerkeliumFactory {

	@Override
	public native Runtime getDefaultRuntime();

	@Override
	public native Runtime createRuntime();
}
