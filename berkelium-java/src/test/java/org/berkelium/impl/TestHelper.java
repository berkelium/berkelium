package org.berkelium.impl;

import org.berkelium.api.BerkeliumFactory;
import org.berkelium.api.Runtime;
import org.berkelium.api.Util;

public class TestHelper {
	public static Integer getObjectCount() {
		return BerkeliumJavaImpl.getObjectCount();
	}

	public static Runtime createRuntime() {
		return Util.createRuntimeWithLog();
	}

	public static String getNativeObjectCount() {
		// load native library
		BerkeliumFactory.getInstance();
		return BerkeliumJavaImpl.getNativeObjectCount();
	}
}
