package org.berkelium.test;

import org.berkelium.api.Tab;
import org.junit.Assert;
import org.junit.Test;

public abstract class AbstractTabTest extends AbstractWindowTest {
	protected Tab tab;

	@Override
	public void before() {
		super.before();
		tab = window.createTab();
		assertChangedAndPush();
	}

	@Override
	public void after() {
		tab.dispose();
		popAndAssert();
		super.after();
	}

	@Test
	public void testTabRuntime() {
		Assert.assertEquals(runtime, tab.getRuntime());
	}
}
