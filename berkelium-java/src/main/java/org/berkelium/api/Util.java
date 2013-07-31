package org.berkelium.api;

public class Util {
	private static boolean loaded = false;

	public static synchronized Runtime createRuntime(String[] args) {
		if (!loaded) {
			try {
				System.loadLibrary("berkelium-java");
				loaded = true;
			} catch (UnsatisfiedLinkError e) {
				System.err.println("Native code library failed to load.\n" + e);
			}
		}
		try {
			Class<?> c = Util.class.getClassLoader().loadClass(
					"org.berkelium.impl.RuntimeImpl");
			return (Runtime) c.getConstructor().newInstance();
		} catch (Exception ex) {
			throw new RuntimeException(ex);
		}
	}

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
		};
	}
}
