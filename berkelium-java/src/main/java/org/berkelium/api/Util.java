package org.berkelium.api;

public class Util {
	/*
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
	*/

	public static LogDelegate log = new LogDelegate() {
		@Override
		public void log(Runtime runtime, LogSource source, LogType type,
			String clazz, String name, String message) {
			System.err.printf("[%s %s] %s\n", clazz, name, message);
		}
	};

	public static Runtime createRuntimeWithLog() {
		return BerkeliumFactory.getInstance().createRuntimeWithLog(log);
	}
}
