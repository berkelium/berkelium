package org.berkelium.java.impl;

import java.io.IOException;
import java.net.InetSocketAddress;
import java.net.ServerSocket;
import java.nio.channels.SelectionKey;
import java.nio.channels.Selector;
import java.nio.channels.ServerSocketChannel;
import java.nio.channels.SocketChannel;
import java.util.HashSet;
import java.util.Iterator;
import java.util.Set;

import org.berkelium.java.api.Berkelium;

public class BerkeliumServer {
	private final static int BASE_PORT = 29050;

	private final Berkelium runtime;
	private final Selector selector;
	private final ServerSocketChannel server;
	private final SelectionKey serverkey;
	private final int port;
	private final Set<SelectionKey> clients = new HashSet<SelectionKey>();
	private boolean connected;

	public BerkeliumServer(Berkelium runtime) {
		this.runtime = runtime;
		try {
			selector = Selector.open();
			server = ServerSocketChannel.open();
			port = bind(server.socket());
			server.configureBlocking(false);
			serverkey = server.register(selector, SelectionKey.OP_ACCEPT);
		} catch (IOException e) {
			throw new RuntimeException(e);
		}
	}

	public int getPort() {
		return port;
	}

	public void update() throws IOException {
		selector.select(100);
		Set<SelectionKey> keys = selector.selectedKeys();

		for(Iterator<SelectionKey> i = keys.iterator(); i.hasNext();) {
			SelectionKey key = i.next();
			i.remove();

			if(key == serverkey) {
				if(key.isAcceptable()) {
					SocketChannel client = server.accept();
					client.configureBlocking(false);
					SelectionKey clientKey = client.register(selector, SelectionKey.OP_READ);
					clients.add(clientKey);
					connected = true;
					System.err.println("new client connected!");
					// ServerChannel ss = new ServerChannel(manager, broadcast, client);
					// clientkey.attach(ss);
					// servers.add(ss);
				}
			} else if(!key.isAcceptable()) {
				System.err.println("client disconnected!");
				key.cancel();
				clients.remove(key);
			} else if(key.isReadable()) {
				// TODO
				// System.err.println("reading...");
			}
		}

		if(connected && clients.isEmpty()) {
			runtime.onCrashed(runtime);
		}
	}

	private static int bind(ServerSocket socket) {
		for(int i = 0; i <= 100; i++) {
			try {
				socket.bind(new InetSocketAddress(BASE_PORT + i));
				return BASE_PORT + i;
			} catch (IOException e) {
				// ignore
			}
		}
		throw new RuntimeException("no free port found!");
	}
}
