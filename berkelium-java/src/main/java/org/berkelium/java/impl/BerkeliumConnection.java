package org.berkelium.java.impl;

import java.io.IOException;
import java.nio.ByteBuffer;
import java.nio.channels.SocketChannel;

public class BerkeliumConnection {
	private final SocketChannel client;
	private final ByteBuffer buffer = ByteBuffer.allocate(1024 * 4);
	private final byte[] array = buffer.array();
	private final WindowImpl window = new WindowImpl();
	private final BerkeliumProcessor processor = new WindowDelegateProcessor(window.getDelegate());

	public BerkeliumConnection(SocketChannel client) {
		this.client = client;
	}

	public boolean read() throws IOException {
		int size = client.read(buffer);
		if(size == -1) {
			return false;
		} else if(size == 0) {
			return true;
		}
		buffer.flip();
		boolean ret = processor.process(array, 0, size);
		buffer.rewind();
		return ret;
	}
}
