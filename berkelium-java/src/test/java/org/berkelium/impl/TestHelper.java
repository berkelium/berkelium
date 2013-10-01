package org.berkelium.impl;

import org.berkelium.api.Runtime;
import org.berkelium.api.Util;

public class TestHelper {
	public static Integer getObjectCount() {
		return BerkeliumJavaImpl.getObjectCount();
	}

	public static Runtime createRuntime() {
		return Util.createRuntimeWithLog();
	}
}
