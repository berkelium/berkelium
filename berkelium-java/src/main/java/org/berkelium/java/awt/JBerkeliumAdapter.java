package org.berkelium.java.awt;

import javax.swing.SwingUtilities;

import org.berkelium.java.api.Berkelium;
import org.berkelium.java.api.BerkeliumDelegate;

public final class JBerkeliumAdapter implements BerkeliumDelegate {
	private final BerkeliumDelegate delegate;

	public JBerkeliumAdapter(BerkeliumDelegate delegate) {
		this.delegate = delegate;
	}

	@Override
	public void onCrashed(final Berkelium runtime) {
		SwingUtilities.invokeLater(new Runnable() {
			@Override
			public void run() {
				delegate.onCrashed(runtime);
			}
		});
	}

	@Override
	public void onClosed(final Berkelium runtime) {
		SwingUtilities.invokeLater(new Runnable() {
			@Override
			public void run() {
				delegate.onClosed(runtime);
			}
		});
	}
}
