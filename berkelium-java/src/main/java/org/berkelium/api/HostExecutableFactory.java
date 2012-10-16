// Copyright (c) 2012 The Berkelium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

package org.berkelium.api;

// =========================================
// WARNING:
//   THIS FILE IS AUTOMATICALLY GENERATED!
//   !! ANY CHANGES WILL BE OVERWRITTEN !!
//
// See berkelium/berkelium-api/update.sh
// =========================================

public class HostExecutableFactory {
	private static HostExecutableFactory instance;

	public synchronized static HostExecutableFactory getInstance() {
		if(instance == null) {
			try {
				Class<?> c = HostExecutableFactory.class.getClassLoader().loadClass(
					"org.berkelium.impl.HostExecutableFactoryImpl");
				instance = (HostExecutableFactory)c.getConstructor().newInstance();
			} catch (Exception ex) {
				throw new RuntimeException(ex);
			}
		}
		return instance;
	}
		
	// Creates an HostExecutable Object representing the given executable.
	HostExecutable forExecutable(String pathTo);

	// Creates an HostExecutable, the exectuable is searched through system path or system configuration (e.g. registry on windows)
	HostExecutable forSystemInstalled();
}
