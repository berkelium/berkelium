package org.berkelium.test;

import org.berkelium.api.BerkeliumFactory;
import org.berkelium.api.HostExecutable;
import org.berkelium.api.Runtime;
import org.junit.Assert;
import org.junit.Test;

public class BerkeliumFactoryTest extends AbstractTest {
	@Test
	public void testRuntimeDispose() {
		Runtime runtime = BerkeliumFactory.getInstance().createRuntime();
		assertChanged();
		runtime.dispose();
	}

	@Test
	public void testForSystemInstalled() {
		Runtime runtime = BerkeliumFactory.getInstance().createRuntime();
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
