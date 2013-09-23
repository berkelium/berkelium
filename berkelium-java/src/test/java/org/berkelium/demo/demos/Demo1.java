package org.berkelium.demo.demos;

import org.berkelium.api.BerkeliumFactory;
import org.berkelium.api.HostExecutable;
import org.berkelium.api.HostVersion;
import org.berkelium.api.Runtime;
import org.berkelium.api.Logger;
import org.berkelium.api.Profile;
import org.berkelium.api.Util;

public class Demo1 {
	public static void dumpProfile(Logger logger, String name, Profile profile) {
		if(profile == null) {
			logger.error(name + " profile not found!");
			return;
		}
		logger.info("profile application: " + profile.getApplicationName());
		logger.debug("profile path: " + profile.getProfilePath());
		logger.debug("profile is found: " + profile.isFound());
		logger.debug("profile in use: " + profile.isInUse());

		profile.dispose();
	}

	public static void main(String[] args) {
		Runtime runtime = BerkeliumFactory.getInstance().createRuntime();

		Logger logger = Util.createRootLogger(runtime);

		logger.info("berkelium demo application...");
		HostExecutable host = runtime.forSystemInstalled();

		HostVersion version = host.getVersion();
		host.dispose();

		logger.info("host version string: " + version);
		version.dispose();

		dumpProfile(logger, "temporary", runtime.createTemporaryProfile());
		dumpProfile(logger, "Chrome", runtime.getChromeProfile());
		dumpProfile(logger, "Chromium", runtime.getChromiumProfile());
		dumpProfile(logger, "Berkelium", runtime.forProfile("berkelium"));
		runtime.dispose();
	}
}
