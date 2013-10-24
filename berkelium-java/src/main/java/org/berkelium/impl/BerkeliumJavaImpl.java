package org.berkelium.impl;

import java.util.HashMap;
import java.util.HashSet;
import java.util.Map;

class BerkeliumJavaImpl {
	private final static Map<Long, BerkeliumObjectImpl> n2j = new HashMap<Long, BerkeliumObjectImpl>();
	private final static Map<BerkeliumObjectImpl, Long> j2n = new HashMap<BerkeliumObjectImpl, Long>();

	public static synchronized native String getNativeObjectCount();

	public static synchronized int getObjectCount() {
		return n2j.size();
	}

	private static Thread shutdownHook = new Thread(){
		public void run() {
			for(BerkeliumObjectImpl o : new HashSet<BerkeliumObjectImpl>(j2n.keySet())) {
				System.err.println("Warning: unclosed Berkelium Object " + o);
				o.dispose();
			}
		}
	};

	private static boolean registerd = false;

	private synchronized static void register() {
		if(!registerd) {
			registerd = true;
			Runtime.getRuntime().addShutdownHook(shutdownHook);
		}
	}

	static synchronized long mapIn(int type, BerkeliumObjectImpl bkJavaId) {
		register();
		if(bkJavaId == null) {
			throw new NullPointerException("BerkeliumObject");
		}
		Long bkNativeId = j2n.get(bkJavaId);
		if(bkNativeId == null) {
			/*
			if(bkJavaId instanceof BerkeliumObjectImpl) {
				System.err.printf("mapIn : %d	j:%s (new)\n", type, bkJavaId.toString());
			} else {
				//throw new IllegalArgumentException("internal error: tried to map none BerkeliumObjectImpl instance of type " +  type + "!");
			}
			*/
			return 0;
		}
		/*
		System.err.printf("mapIn : %d	j:0x%x = id:0x%x\n", type, bkJavaId.hashCode(), bkNativeId);
		*/
		return bkNativeId;
		/*
		Long obj = n2o.get(bkNativeId);
		System.err.printf("mapIn : %d	j:0x%x = id:%x obj=%x\n", type, bkJavaId.getBerkeliumId(), bkNativeId, obj);
		return obj;
		*/
	}

	static synchronized BerkeliumObjectImpl mapOut(int type, long bkNativeId) {
		register();
		BerkeliumObjectImpl bkJavaId = n2j.get(bkNativeId);
		/*
		if(bkJavaId == null) {
			System.err.printf("mapOut: %d	id:0x%x (calling mapNew)\n", type, bkNativeId);
		} else {
			System.err.printf("mapOut: %d	id:0x%x = j:0x%x(old)\n", type, bkNativeId, bkJavaId.toString());
		}
		*/
		return bkJavaId;
	}

	static synchronized void mapNew(int type, long bkNativeId, BerkeliumObjectImpl bkJavaId) {
		if(bkJavaId == null) {
			throw new NullPointerException("BerkeliumObject");
		}
		/*
		System.err.printf("mapNew: %d	id:0x%x = j:%s\n", type, bkNativeId, bkJavaId.toString());
		*/
		n2j.put(bkNativeId, bkJavaId);
		j2n.put(bkJavaId, bkNativeId);
	}

	static synchronized void free(long bkNativeId) {
		BerkeliumObjectImpl removed = n2j.remove(bkNativeId);
		/*
		System.err.printf("free: id:0x%x = j:%s\n", bkNativeId, removed);
		*/
		if(removed != null) {
			j2n.remove(removed);
		}
	}
}
