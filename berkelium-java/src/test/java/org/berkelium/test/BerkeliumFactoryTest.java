package org.berkelium.test;

import org.berkelium.api.HostExecutable;
import org.berkelium.api.Runtime;
import org.berkelium.impl.TestHelper;
import org.junit.Assert;
import org.junit.Test;

public class BerkeliumFactoryTest extends AbstractTest {
	@Test
	public void testRuntimeDispose() {
		Runtime runtime = TestHelper.createRuntime();
		assertChanged();
		runtime.dispose();
	}

	@Test
	public void testForSystemInstalled() {
		Runtime runtime = TestHelper.createRuntime();
		assertChangedAndPush();
		{
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
		runtime.dispose();
		popAndAssert();
	}
}
