package org.berkelium.java.impl;

import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.IOException;

public abstract class BerkeliumProcessor {
	private final FifoStream fifo = new FifoStream(4 * 1024 * 1024);
	private final DataInputStream dis = fifo.getDataInputStream();
	private final DataOutputStream dos = fifo.getDataOutputStream();
	private int toRead = 0;

	public boolean process(byte[] b, int off, int len) throws IOException {
		if(len == 0) {
			return true;
		}
		dos.write(b, off, len);
		int a = dis.available();
		if(toRead == 0 && a >= 4) {
			toRead = dis.readInt();
			a -= 4;
		}
		if(toRead == 4) {
			// disconnect command
			return false;
		}
		int d = a - toRead;
		if(toRead != 0 && d >= 0) {
			process(dis);
			while(dis.available() < d) {
				System.err.println("Warning: skipping byte!");
				dis.read();
			}
			toRead = 0;
		}
		return true;
	}

	protected abstract void process(DataInputStream data) throws IOException;

	public int available() {
		return fifo.available();
	}
}
