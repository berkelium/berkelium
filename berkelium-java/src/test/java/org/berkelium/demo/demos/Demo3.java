package org.berkelium.demo.demos;

import org.berkelium.api.HostExecutable;
import org.berkelium.api.Instance;
import org.berkelium.api.Logger;
import org.berkelium.api.Profile;
import org.berkelium.api.Rect;
import org.berkelium.api.Runtime;
import org.berkelium.api.Tab;
import org.berkelium.api.TabDelegate;
import org.berkelium.api.Util;
import org.berkelium.api.Window;

public class Demo3 {
	private final static TabDelegate tabDelegate = new TabDelegate() {
		@Override
		public void onTitleChanged(Tab tab, String title) {
			System.err.println("onTitleChanged " + title);
		}

		@Override
		public void onPaintDone(Tab tab, Rect rect) {
			System.err.println("onPaintDone");
		}

		@Override
		public void onPaint(Tab tab) {
			System.err.println("onPaint");
		}

		@Override
		public void onClosed(Tab tab) {
			System.err.println("onClosed");
		}

		@Override
		public void onReady(Tab tab) {
			System.err.println("onReady");
		}
	};

	public static void main(String[] args) throws InterruptedException {
		Runtime runtime = Util.createRuntimeWithLog();

		Logger logger = runtime.getLogger(Demo3.class.getName(), "");

		logger.info("berkelium demo application...");
		HostExecutable host = runtime.forSystemInstalled();

		if(host == null) {
			logger.error("berkelium host executable not found!");
			return;
		}

		Profile profile = runtime.createTemporaryProfile();
		logger.info("starting berkelium browser...");

		Instance instance = runtime.open(host, profile);
		if(instance == null) {
			logger.info("berkelium browser can not be started!");
			return;
		}
		logger.info("berkelium browser is running!");

		Window win = instance.createWindow(false);
		Tab tab = win.createTab();

		tab.addTabDelegate(tabDelegate);

		logger.info("waiting 10s...");

		for(int i = 0; i < 10000; i += 100) {
			runtime.update(100);
		}

		logger.info("shutting down browser...");

		instance.close();

		logger.info("dispose tab...");
		tab.dispose();

		logger.info("dispose window...");
		win.dispose();

		logger.info("dispose instance...");
		instance.dispose();

		logger.info("dispose profile...");
		profile.dispose();

		logger.info("dispose host...");
		host.dispose();

		logger.info("dispose runtime...");
		runtime.dispose();

		logger.info("disposed!");

		logger.dispose();
	}
}
