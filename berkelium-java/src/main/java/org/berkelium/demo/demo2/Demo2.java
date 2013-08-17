package org.berkelium.demo.demo2;

import org.berkelium.api.BerkeliumFactory;
import org.berkelium.api.HostExecutable;
import org.berkelium.api.Instance;
import org.berkelium.api.Runtime;
import org.berkelium.api.Logger;
import org.berkelium.api.Profile;
import org.berkelium.api.Tab;
import org.berkelium.api.Util;
import org.berkelium.api.Window;

public class Demo2 {
	public static void main(String[] args) throws InterruptedException {
		Runtime runtime = BerkeliumFactory.getInstance().createRuntime();

		Logger logger = Util.createRootLogger(runtime);

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

		logger.info("waiting 10s...");

		for(int i = 0; i < 10000; i += 100) {
			instance.internalUpdate();
			Thread.sleep(100);
		}

		logger.info("shutting down browser...");

		instance.close();

		logger.info("done!");
	}
}
