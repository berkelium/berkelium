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
		while(true) {
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
				int skip = dis.available() - d;
				if(skip > 0) {
					System.err.printf("skipping %d bytes\n", skip);
					dis.skipBytes(skip);
				}
				toRead = 0;
			} else {
				return true;
			}
		}
	}

	protected abstract void process(DataInputStream data) throws IOException;

	public int available() {
		return fifo.available();
	}
}
