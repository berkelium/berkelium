package org.berkelium.impl;

import java.util.HashMap;
import java.util.Map;
import java.util.concurrent.atomic.AtomicInteger;

public class BerkeliumJavaImpl {
	private final static Map<Long, BerkeliumObject> n2j = new HashMap<Long, BerkeliumObject>();
	private final static Map<BerkeliumObject, Long> j2n = new HashMap<BerkeliumObject, Long>();
	private final static Map<Long, Long> n2o = new HashMap<Long, Long>();

	static synchronized long mapIn(int type, BerkeliumObject bkJavaId) {
		if(bkJavaId == null) {
			throw new NullPointerException("BerkeliumObject");
		}
		Long bkNativeId = j2n.get(bkJavaId);
		if(bkNativeId == null) {
			//System.err.printf("mapIn : %d	j:0x%x (new)\n", type, bkJavaId.getBerkeliumId());
			return 0;
		}
		Long obj = n2o.get(bkNativeId);
		//System.err.printf("mapIn : %d	j:0x%x = id:%x obj=%x\n", type, bkJavaId.getBerkeliumId(), bkNativeId, obj);
		return obj;
	}

	static synchronized BerkeliumObject mapOut(int type, long bkNativeId) {
		BerkeliumObject bkJavaId = n2j.get(bkNativeId);
		/*
		if(bkJavaId == null) {
			System.err.printf("mapOut: %d	id:0x%x (calling mapNew)\n", type, bkNativeId);
		} else {
			System.err.printf("mapOut: %d	id:0x%x = j:0x%x(old)\n", type, bkNativeId, bkJavaId.getBerkeliumId());
		}
		*/
		return bkJavaId;
	}

	static synchronized void mapNew(int type, long bkNativeId, long obj, BerkeliumObject bkJavaId) {
		if(bkJavaId == null) {
			throw new NullPointerException("BerkeliumObject");
		}
		//System.err.printf("mapNew: %d	id:0x%x = j:0x%x(new)\n", type, bkNativeId, bkJavaId.getBerkeliumId());
		n2o.put(bkNativeId, obj);
		n2j.put(bkNativeId, bkJavaId);
		j2n.put(bkJavaId, bkNativeId);
	}

	private final static AtomicInteger idCounter = new AtomicInteger();

	static int createId() {
		return idCounter.getAndIncrement();
	}
}
