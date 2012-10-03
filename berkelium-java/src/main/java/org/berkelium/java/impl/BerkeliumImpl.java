package org.berkelium.java.impl;

import java.io.IOException;
import java.util.LinkedHashSet;
import java.util.Set;
import java.util.concurrent.CyclicBarrier;
import java.util.concurrent.atomic.AtomicBoolean;

import org.berkelium.java.api.Berkelium;
import org.berkelium.java.api.BerkeliumDelegate;
import org.berkelium.java.api.BerkeliumFactory;
import org.berkelium.java.api.Window;

public final class BerkeliumImpl implements Berkelium, Runnable {
	private final Set<BerkeliumDelegate> delegates = new LinkedHashSet<BerkeliumDelegate>();
	private final CyclicBarrier initDoneBarrier = new CyclicBarrier(2);
	private final AtomicBoolean destroyed = new AtomicBoolean(false);
	private final AtomicBoolean updateRunning = new AtomicBoolean(false);
	private final Thread thread;
	private final BerkeliumServer server;
	private final BerkeliumProcess process;

	private RuntimeException initRuntimeException;

	BerkeliumImpl(String app, String profile) {
		server = new BerkeliumServer(this);
		System.err.println("listing on port " + server.getPort());
		process = new BerkeliumProcess(server.getPort(), this);
		System.err.println("berkelium is running!");

		thread = new Thread(this, "berkelium thread " + app + " " + profile);
		thread.setDaemon(true);
		thread.start();
		try {
			initDoneBarrier.await();
		} catch (Exception e) {
			handleThrowable(e);
		}
		if(initRuntimeException != null)
			throw initRuntimeException;
	}

	@Override
	public void run() {
		try {
			initThread();
		} catch (Exception e) {
			initRuntimeException = new RuntimeException(e);
			return;
		} finally {
			try {
				initDoneBarrier.await();
			} catch (Throwable t) {
				handleThrowable(t);
				throw new RuntimeException(t);
			}
		}
		try {
			while(!destroyed.get()) {
				update();
			}
		} catch (Throwable t) {
			handleThrowable(t);
			throw new RuntimeException(t);
		} finally {
			shutdownThread();
		}
	}

	private void initThread() {
	}

	private void shutdownThread() {
		process.destroy();
	}

	private void update() throws IOException {
		if(updateRunning.getAndSet(true))
			throw new IllegalStateException("update() can not call itself!");

		try {
			process.update();
			server.update();
			/*
			byte[] bytes = ipc.readPackage();
			if(bytes != null) {
				root.dispatch(new PackageReader(ipc, bytes));
			}
			*/
		} finally {
			updateRunning.set(false);
		}
	}

	private void checkNotDestoryed() {
		if(destroyed.get()) {
			throw new IllegalStateException("already destroyed!");
		}
	}

	@Override
	public synchronized void close() {
		BerkeliumFactory.closeInstance(this);
	}

	public synchronized void destory() {
		destroyed.set(true);
	}

	public final static WindowImpl onlyOneWindowSupported = new WindowImpl();

	@Override
	public Window createWindow() {
		checkNotDestoryed();
		return onlyOneWindowSupported;
		/*
		return new WindowImpl();
		*/
	}

	@Override
	public void sync(Window window) {
		checkNotDestoryed();
	}

	@Override
	public void handleThrowable(Throwable t) {
	}

	@Override
	public void log(String message) {
		System.err.println(message);
	}

	private Set<BerkeliumDelegate> getDelegates() {
		synchronized(delegates) {
			return new LinkedHashSet<BerkeliumDelegate>(delegates);
		}
	}

	@Override
	public void onCrashed(Berkelium runtime) {
		for(BerkeliumDelegate delegate : getDelegates()) {
			delegate.onCrashed(runtime);
		}
		close();
	}

	@Override
	public void onClosed(Berkelium runtime) {
		for(BerkeliumDelegate delegate : getDelegates()) {
			delegate.onCrashed(runtime);
		}
	}

	@Override
	public void addDelegate(BerkeliumDelegate delegate) {
		synchronized(delegates) {
			delegates.add(delegate);
		}
	}

	@Override
	public void removeDelegate(BerkeliumDelegate delegate) {
		synchronized(delegates) {
			delegates.remove(delegate);
		}
	}
}
