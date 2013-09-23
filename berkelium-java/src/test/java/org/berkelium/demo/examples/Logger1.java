// Copyright (c) 2013 The Berkelium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

package org.berkelium.demo.examples;

import org.berkelium.api.BerkeliumFactory;
import org.berkelium.api.LogDelegate;
import org.berkelium.api.LogSource;
import org.berkelium.api.LogType;
import org.berkelium.api.Logger;
import org.berkelium.api.Runtime;

public class Logger1 {
	// This LogDelegate will receive all Log Messages from Berkelium
	private static /*GREEN*/LogDelegate log/*GREEN*/ = new LogDelegate() {
		@Override
		public void log(Runtime runtime, LogSource source, LogType type,
			String clazz, String name, String message) {
			// just a demo: write log message to System.err...
			/*GREEN*/System.err.printf("%s %s %s\n", clazz, name, message);/*GREEN*/
		}
	};

	public static void main(String[] args) {
		// Create Runtime, use given Log Delegate
		Runtime runtime = /*GREEN*/BerkeliumFactory.getInstance().createRuntimeWithLog(log);/*GREEN*/

		/*BLUE*/// The Logger Code here is only necessary to demonstrate an invocation of the LogDelegate/*BLUE*/
		/*BLUE*/// Berkelium itself creates log messages that are delegated to the Log Delegate/*BLUE*/
		/*BLUE*/// Because of this it is not necessary to use the Logger Classes directly/*BLUE*/
		Logger logger = runtime.getLogger(Logger1.class.getName(), "");
		// Create a Log Message to demonstrate the LogDelegate
		logger.info("Hello World!");

		// close logger
		logger.dispose();

		// close runtime
		runtime.dispose();
	}
}
