package org.berkelium.java.impl;

import java.util.HashSet;

import org.berkelium.java.api.Berkelium;
import org.berkelium.java.api.BerkeliumFactory;

public class BerkeliumFactoryImpl extends BerkeliumFactory {
	private final static HashSet<BerkeliumImpl> instances = new HashSet<BerkeliumImpl>();

	private final static Thread shutdown = new Thread("Berkelium Shutdown Thread") {
		public synchronized void run() {
			shutdown();
		}
	};

	static {
		Runtime.getRuntime().addShutdownHook(shutdown);
	}

	@Override
	protected Berkelium openInstanceImpl(String app, String profile) {
		BerkeliumImpl ret = new BerkeliumImpl(app, profile);
		synchronized(instances) {
			instances.add(ret);
		}
		return ret;
	}

	@Override
	protected void closeInstanceImpl(Berkelium instance) {
		synchronized(instances) {
			instances.remove(instance);
			((BerkeliumImpl)instance).destory();
		}
	}

	private static void shutdown() {
		System.err.println("shutting down berkelium!");
		while(true) {
			Berkelium berkelium;
			synchronized(instances) {
				if(instances.isEmpty()) {
					break;
				}
				berkelium = instances.iterator().next();
			}
			berkelium.close();
		}
		System.err.println("berkelium closed!");
	}
}
