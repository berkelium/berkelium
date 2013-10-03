package org.berkelium.test;

import org.berkelium.api.HostExecutable;
import org.berkelium.api.Instance;
import org.berkelium.api.Profile;
import org.berkelium.api.Runtime;
import org.berkelium.api.Tab;
import org.berkelium.api.TabDelegate;
import org.berkelium.api.Window;
import org.berkelium.impl.TestHelper;
import org.junit.After;
import org.junit.Assert;
import org.junit.Before;
import org.junit.Test;
import org.junit.runner.RunWith;
import org.mockito.Mock;
import org.mockito.runners.MockitoJUnitRunner;

@RunWith(MockitoJUnitRunner.class)
public class TabDelegateTest extends AbstractTest {
	private Runtime runtime;
	private HostExecutable host;
	private Profile profile;
	private Instance instance;
	private Window window;
	private Tab tab;

	@Mock
	private TabDelegate delegate;

	@Before
	public void before() {
		runtime = TestHelper.createRuntime();
		assertChangedAndPush();
		host = runtime.forSystemInstalled();
		assertChangedAndPush();
		profile = runtime.createTemporaryProfile();
		assertChangedAndPush();
		instance = runtime.open(host, profile);
		assertChangedAndPush();
		window = instance.createWindow(true);
		assertChangedAndPush();
		tab = window.createTab();
		assertChangedAndPush();
		tab.getRuntime();
		tab.addTabDelegate(delegate);
	}

	@After
	public void after() {
		tab.dispose();
		popAndAssert();
		window.dispose();
		popAndAssert();
		instance.dispose();
		popAndAssert();
		host.dispose();
		popAndAssert();
		profile.dispose();
		popAndAssert();
		runtime.dispose();
	}

	@Test
	public void testRuntime() {
		Assert.assertSame(runtime, tab.getRuntime());
	}

	@Test
	public void testOnReady() {
		runtime.update(5000);
	}
}
