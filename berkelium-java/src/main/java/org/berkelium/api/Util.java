package org.berkelium.api;

public class Util {
	public static Logger createRootLogger(Runtime runtime) {
		return new Logger() {
			@Override
			public void debug(String message) {
				System.err.println(message);
			}

			@Override
			public void info(String message) {
				System.err.println(message);
			}

			@Override
			public void warn(String message) {
				System.err.println(message);
			}

			@Override
			public void error(String message) {
				System.err.println(message);
			}

			@Override
			public void dispose() {
			}
		};
	}
}
