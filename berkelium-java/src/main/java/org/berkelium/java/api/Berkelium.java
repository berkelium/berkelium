package org.berkelium.java.api;

public interface Berkelium extends LogHandler, BerkeliumDelegate {
	Window createWindow();

	void sync(Window window);

	void close();

	void handleThrowable(Throwable t);

	void addDelegate(BerkeliumDelegate delegate);

	void removeDelegate(BerkeliumDelegate delegate);
}
