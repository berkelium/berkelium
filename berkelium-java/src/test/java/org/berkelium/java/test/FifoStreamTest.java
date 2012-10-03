package org.berkelium.java.test;

import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.IOException;

import org.berkelium.java.impl.FifoStream;
import org.junit.Assert;
import org.junit.Test;

public class FifoStreamTest {
	private final static int size = 4096;

	@Test
	public void test0() throws IOException {
		FifoStream fifo = new FifoStream(size);
		DataInputStream dis = fifo.getDataInputStream();
		DataOutputStream dos = fifo.getDataOutputStream();
		dos.writeUTF("test UTF8 €...");
		Assert.assertEquals("test UTF8 €...", dis.readUTF());
		dos.writeByte(-1);
		Assert.assertEquals(-1, dis.readByte());
		dos.writeShort(-1);
		Assert.assertEquals(-1, dis.readShort());
		dos.writeInt(-1);
		Assert.assertEquals(-1, dis.readInt());
	}

	@Test
	public void test1() throws IOException {
		for(int size = 1; size < 1024; size++) {
			FifoStream fifo = new FifoStream(size);
			DataInputStream dis = fifo.getDataInputStream();
			DataOutputStream dos = fifo.getDataOutputStream();

			check(size, fifo, dis, 0);
			for(int i = 1; i <= size; i++) {
				dos.write(0);
				check(size, fifo, dis, i);
			}
			check(size, fifo, dis, size);
			for(int i = size; i <= 0; i--) {
				dis.read();
				check(size, fifo, dis, i);
			}
			check(size, fifo, dis, size);
		}
	}

	@Test
	public void test2() throws IOException {
		FifoStream fifo = new FifoStream(size);
		DataInputStream dis = fifo.getDataInputStream();
		DataOutputStream dos = fifo.getDataOutputStream();

		check(size, fifo, dis, 0);
		for(int i = 1; i <= size / 2; i++) {
			dos.write(0);
			check(size, fifo, dis, i);
			dos.write(0);
			check(size, fifo, dis, i + 1);
			dis.read();
			check(size, fifo, dis, i);
		}
	}

	@Test
	public void test3() throws IOException {
		FifoStream fifo = new FifoStream(size);
		DataInputStream dis = fifo.getDataInputStream();
		DataOutputStream dos = fifo.getDataOutputStream();

		for(int i = 1; i <= size; i++) {
			dos.write(i);
		}
		for(int i = 1; i <= size; i++) {
			Assert.assertEquals(i & 0xFF, dis.read());
		}
	}

	private void check(int size, FifoStream fifo, DataInputStream dis, int a) throws IOException {
		Assert.assertEquals("size " + size + " available", a, dis.available());
		Assert.assertEquals("size " + size + " remaining", size - a, fifo.remaining());
	}
}
