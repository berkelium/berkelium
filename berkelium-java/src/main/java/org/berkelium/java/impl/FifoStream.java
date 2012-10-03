package org.berkelium.java.impl;

import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;

public class FifoStream {
	private final int max;
	private final byte[] array;
	private int readPos = 0;
	private int writePos = 0;

	public class FifoOutputStream extends OutputStream {
		@Override
		public void write(int b) throws IOException {
			synchronized(array) {
				if(writePos > max) {
					writePos = 0;
				}
				if(readPos == writePos + 1) {
					throw new IOException("buffer overflow!");
				}
				array[writePos] = (byte)b;
				writePos++;
			}
		}
	}

	public class FifoInputStream extends InputStream {
		@Override
		public int read() throws IOException {
			synchronized(array) {
				if(readPos == writePos) {
					throw new IOException("no data available");
				}
				int ret = array[readPos];
				readPos++;
				if(readPos > max) {
					readPos = 0;
				}
				return ret;
			}
		}

		@Override
		public int available() {
			synchronized(array) {
				if(writePos < readPos) {
					return max + writePos - readPos;
				} else {
					return writePos - readPos;
				}
			}
		}
	}

	private final FifoOutputStream fos = new FifoOutputStream();
	private final FifoInputStream fis = new FifoInputStream();
	private final DataOutputStream dos = new DataOutputStream(fos);
	private final DataInputStream dis = new DataInputStream(fis);

	public FifoStream(int max) {
		this.max = max;
		array = new byte[max];
	}

	public DataInputStream getDataInputStream() {
		return dis;
	}

	public DataOutputStream getDataOutputStream() {
		return dos;
	}

	public int available() {
		return fis.available();
	}

	public int remaining() {
		synchronized(array) {
			return max - fis.available();
		}
	}
}
