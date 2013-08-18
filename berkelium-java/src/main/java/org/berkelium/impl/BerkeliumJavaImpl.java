package org.berkelium.impl;

import java.util.HashMap;
import java.util.Map;
import java.util.concurrent.atomic.AtomicInteger;

public class BerkeliumJavaImpl {
	private final static Map<Long, BerkeliumObjectImpl> n2j = new HashMap<Long, BerkeliumObjectImpl>();
	private final static Map<BerkeliumObjectImpl, Long> j2n = new HashMap<BerkeliumObjectImpl, Long>();

	static synchronized long mapIn(int type, BerkeliumObjectImpl bkJavaId) {
		if(bkJavaId == null) {
			throw new NullPointerException("BerkeliumObject");
		}
		Long bkNativeId = j2n.get(bkJavaId);
		/*
		if(bkNativeId == null) {
			System.err.printf("mapIn : %d	j:0x%x (new)\n", type, bkJavaId.getBerkeliumId());
			return 0;
		}
		System.err.printf("mapIn : %d	j:0x%x = id:0x%x\n", type, bkJavaId.getBerkeliumId(), bkNativeId);
		*/
		return bkNativeId;
		/*
		Long obj = n2o.get(bkNativeId);
		System.err.printf("mapIn : %d	j:0x%x = id:%x obj=%x\n", type, bkJavaId.getBerkeliumId(), bkNativeId, obj);
		return obj;
		*/
	}

	static synchronized BerkeliumObjectImpl mapOut(int type, long bkNativeId) {
		BerkeliumObjectImpl bkJavaId = n2j.get(bkNativeId);
		/*
		if(bkJavaId == null) {
			System.err.printf("mapOut: %d	id:0x%x (calling mapNew)\n", type, bkNativeId);
		} else {
			System.err.printf("mapOut: %d	id:0x%x = j:0x%x(old)\n", type, bkNativeId, bkJavaId.getBerkeliumId());
		}
		*/
		return bkJavaId;
	}

	static synchronized void mapNew(int type, long bkNativeId, BerkeliumObjectImpl bkJavaId) {
		if(bkJavaId == null) {
			throw new NullPointerException("BerkeliumObject");
		}
		/*
		System.err.printf("mapNew: %d	id:0x%x = j:0x%x(new)\n", type, bkNativeId, bkJavaId.getBerkeliumId());
		*/
		n2j.put(bkNativeId, bkJavaId);
		j2n.put(bkJavaId, bkNativeId);
	}

	private final static AtomicInteger idCounter = new AtomicInteger();

	static int createId() {
		return idCounter.getAndIncrement();
	}
}
