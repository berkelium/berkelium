package org.berkelium.java.api;

public abstract class BerkeliumFactory {
	private static BerkeliumFactory instance;

	private synchronized static BerkeliumFactory getInstance() {
		if(instance == null) {
			try {
				Class<?> c = BerkeliumFactory.class.getClassLoader().loadClass(
					"org.berkelium.java.impl.BerkeliumFactoryImpl");
				instance = (BerkeliumFactory)c.getConstructor().newInstance();
			} catch (Exception ex) {
				throw new RuntimeException(ex);
			}
		}
		return instance;
	}

	protected abstract Berkelium openInstanceImpl(String app, String profile);

	protected abstract void closeInstanceImpl(Berkelium instance);

	public static Berkelium openInstance(String app) {
		return getInstance().openInstanceImpl(app, null);
	}

	public static Berkelium openInstance(String app, String profile) {
		return getInstance().openInstanceImpl(app, profile);
	}

	public static Berkelium createTemporaryInstance() {
		return getInstance().openInstanceImpl(null, null);
	}

	public static void closeInstance(Berkelium instance) {
		getInstance().closeInstanceImpl(instance);
	}
}
