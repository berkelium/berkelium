package org.berkelium.java.impl;

import org.berkelium.java.api.Window;
import org.berkelium.java.api.WindowDelegate;

public class WindowImpl implements Window {
	private final MultiDelegate delegate = new MultiDelegate();

	public WindowDelegate getDelegate() {
		return delegate.getProxy();
	}

	@Override
	public void addDelegate(WindowDelegate wd) {
		delegate.addDelegate(wd);
	}

	@Override
	public void removeDelegate(WindowDelegate wd) {
		delegate.removeDelegate(wd);
	}

	@Override
	public void resize(int width, int height) {
	}

	@Override
	public boolean navigateTo(String url) {
		return false;
	}
}
