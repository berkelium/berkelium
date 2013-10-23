package org.berkelium.test;

import org.berkelium.api.HostExecutable;
import org.berkelium.api.Profile;
import org.junit.Assert;
import org.junit.Test;

public class RuntimeTest extends AbstractRuntimeTest {
	/*
	@Test
	@Ignore
	public void testSecondRuntimeDispose() {
		// AbstractRuntimeTest tracks all objects.
		// for every Runtime a new Tracker is needed.
		// Because of that this can't work...
		Runtime runtime = TestHelper.createRuntime();
		assertChanged();
		runtime.dispose();
	}
	*/

	@Test
	public void testForSystemInstalled() {
		HostExecutable host = runtime.forSystemInstalled();
		assertChangedAndPush();
		{
			Assert.assertNotNull(host);
			/* TODO
			HostVersion version = host.getVersion();
			assertChangedAndPush();
			{
				Assert.assertNotNull(version);
			}
			version.dispose();
			popAndAssert();
			*/
		}
		host.dispose();
		popAndAssert();
	}

	@Test
	public void testCreateTemporaryProfile() {
		Profile profile = runtime.createTemporaryProfile();
		assertChangedAndPush();
		profile.dispose();
		popAndAssert();
	}
}
