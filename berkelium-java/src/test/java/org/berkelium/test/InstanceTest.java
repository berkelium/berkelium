package org.berkelium.test;

import org.berkelium.api.Window;
import org.junit.Assert;
import org.junit.Test;

public class InstanceTest extends AbstractInstanceTest {
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
