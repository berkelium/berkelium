package org.berkelium.test;

import org.berkelium.api.BerkeliumFactory;
import org.berkelium.api.HostExecutable;
import org.berkelium.api.Instance;
import org.berkelium.api.Profile;
import org.berkelium.api.Runtime;
import org.berkelium.api.Window;
import org.junit.After;
import org.junit.Assert;
import org.junit.Before;
import org.junit.Test;

public class InstanceTest extends AbstractTest {
	private Runtime runtime;
	private HostExecutable host;
	private Profile profile;
	private Instance instance;

	@Before
	public void before() {
		runtime = BerkeliumFactory.getInstance().createRuntime();
		assertChangedAndPush();
		host = runtime.forSystemInstalled();
		assertChangedAndPush();
		profile = runtime.createTemporaryProfile();
		assertChangedAndPush();
		instance = runtime.open(host, profile);
		assertChangedAndPush();
	}

	@After
	public void after() {
		instance.dispose();
		popAndAssert();
		host.dispose();
		popAndAssert();
		profile.dispose();
		popAndAssert();
		runtime.dispose();
	}

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
