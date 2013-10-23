package org.berkelium.test;

import org.berkelium.api.Window;
import org.junit.Assert;
import org.junit.Test;

public abstract class AbstractWindowTest extends AbstractInstanceTest {
	protected Window window;

	protected boolean createIncognitoWindow() {
		return true;
	}

	@Override
	public void before() {
		super.before();
		window = instance.createWindow(createIncognitoWindow());
		assertChangedAndPush();
	}

	@Override
	public void after() {
		window.dispose();
		popAndAssert();
		super.after();
	}

	@Test
	public void testWindowRuntime() {
		Assert.assertEquals(runtime, window.getRuntime());
	}
}
