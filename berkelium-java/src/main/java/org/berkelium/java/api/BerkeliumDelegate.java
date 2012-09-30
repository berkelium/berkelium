package org.berkelium.java.api;

public interface BerkeliumDelegate {
	void onCrashed(Berkelium runtime);

	void onClosed(Berkelium runtime);
}
