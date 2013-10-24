package org.berkelium.test;

import org.berkelium.api.HostExecutable;
import org.berkelium.api.Instance;
import org.berkelium.api.Profile;
import org.junit.Assert;
import org.junit.Test;

public abstract class AbstractInstanceTest extends AbstractRuntimeTest {
	protected HostExecutable host;
	protected Profile profile;
	protected Instance instance;

	@Override
	public void before() {
		super.before();
		host = runtime.forSystemInstalled();
		assertChangedAndPush();
		profile = runtime.createTemporaryProfile();
		assertChangedAndPush();
		instance = runtime.open(host, profile);
		assertChangedAndPush();
	}

	@Override
	public void after() {
		instance.dispose();
		popAndAssert();
		profile.dispose();
		popAndAssert();
		host.dispose();
		popAndAssert();
		super.after();
	}

	@Test
	public void testInstanceRuntime() {
		Assert.assertEquals(runtime, instance.getRuntime());
	}

	@Test
	public void testHostRuntime() {
		Assert.assertEquals(runtime, host.getRuntime());
	}

	@Test
	public void testProfileRuntime() {
		Assert.assertEquals(runtime, profile.getRuntime());
	}
}
