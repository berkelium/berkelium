package org.berkelium.test;

import org.berkelium.api.HostDelegate;
import org.berkelium.api.Window;
import org.junit.Assert;
import org.junit.Test;
import org.junit.runner.RunWith;
import org.mockito.Mock;
import org.mockito.runners.MockitoJUnitRunner;

@RunWith(MockitoJUnitRunner.class)
public class HostDelegateTest extends AbstractInstanceTest {
	@Mock
	private HostDelegate delegate;

	@Override
	public void before() {
		super.before();
		instance.addHostDelegate(delegate);
		//assertChangedAndPush();
	}

/*
	@Override
	public void after() {
		//instance.removeHostDelegate(delegate);
		//popAndAssert();
		super.after();
	}
*/

	@Test
	public void testCreateWindow() {
		Window win = instance.createWindow(false);
		assertChangedAndPush();
		{
			Assert.assertNotNull(win);
		}
		win.dispose();
		popAndAssert();
	}
}
