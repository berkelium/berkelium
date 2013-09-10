// Copyright (c) 2013 The Berkelium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

package org.berkelium.demo.runtime;

import org.berkelium.api.BerkeliumFactory;
import org.berkelium.api.Runtime;

public class Runtime2 {
	public static void main(String[] args) {
		// Create Runtime
		Runtime runtime = BerkeliumFactory.getInstance().createRuntime();
		/*GREEN*/// close runtime/*GREEN*/
		/*GREEN*/runtime.dispose();/*GREEN*/
	}
}
