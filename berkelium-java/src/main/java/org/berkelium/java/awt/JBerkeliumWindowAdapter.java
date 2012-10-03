package org.berkelium.java.awt;

import java.lang.reflect.InvocationHandler;
import java.lang.reflect.InvocationTargetException;
import java.lang.reflect.Method;
import java.lang.reflect.Proxy;
import java.util.concurrent.atomic.AtomicReference;

import javax.swing.SwingUtilities;

import org.berkelium.java.api.WindowDelegate;

public final class JBerkeliumWindowAdapter implements InvocationHandler {
	private final WindowDelegate delegate;
	private final WindowDelegate proxy = (WindowDelegate)Proxy.newProxyInstance(getClass().getClassLoader(),
		new Class<?>[] {
			WindowDelegate.class
		}, this);

	public JBerkeliumWindowAdapter(WindowDelegate delegate) {
		this.delegate = delegate;
	}

	public WindowDelegate getProxy() {
		return proxy;
	}

	public Object invoke(Object proxy, final Method method, final Object[] args) throws Throwable {
		final AtomicReference<Object> ret = new AtomicReference<Object>();
		final AtomicReference<InvocationTargetException> ex = new AtomicReference<InvocationTargetException>();
		if(method.getReturnType() == void.class) {
			SwingUtilities.invokeLater(new Runnable() {
				@Override
				public void run() {
					try {
						method.invoke(delegate, args);
					} catch (IllegalArgumentException e) {
						// TODOBerkelium.handleThrowable(th);
					} catch (IllegalAccessException e) {
						// TODOBerkelium.handleThrowable(th);
					} catch (InvocationTargetException e) {
						ex.set(e);
					}
				}
			});
		} else {
			SwingUtilities.invokeAndWait(new Runnable() {
				@Override
				public void run() {
					try {
						ret.set(method.invoke(delegate, args));
					} catch (IllegalArgumentException e) {
						// TODOBerkelium.handleThrowable(th);
					} catch (IllegalAccessException e) {
						// TODOBerkelium.handleThrowable(th);
					} catch (InvocationTargetException e) {
						ex.set(e);
					}
				}
			});
		}
		InvocationTargetException e = ex.get();
		if(e != null) {
			throw new RuntimeException(e);
		}
		return ret.get();
	}
}
