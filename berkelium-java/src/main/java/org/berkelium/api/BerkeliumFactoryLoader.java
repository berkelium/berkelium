package org.berkelium.api;

import java.lang.reflect.Constructor;

final class BerkeliumFactoryLoader {
	private static BerkeliumFactory instance;

	synchronized static BerkeliumFactory getBerkeliumFactoryInstance() {
		if (instance == null) {
			try {
				System.loadLibrary("berkelium-java" + System.getProperty("sun.arch.data.model"));
				Class<?> c = BerkeliumFactory.class.getClassLoader().loadClass(
						"org.berkelium.impl.BerkeliumFactoryImpl");
				Constructor<?> ctor = c.getConstructor();
				ctor.setAccessible(true);
				instance = (BerkeliumFactory) ctor.newInstance();
			} catch (Exception ex) {
				throw new RuntimeException(ex);
			}
		}
		return instance;
	}
}
