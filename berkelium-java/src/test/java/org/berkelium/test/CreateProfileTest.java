package org.berkelium.test;

import org.berkelium.api.BerkeliumFactory;
import org.berkelium.api.Profile;
import org.berkelium.api.Runtime;
import org.junit.Assert;
import org.junit.Test;

public class CreateProfileTest extends AbstractTest {
	interface GetProfile {
		Profile getProfile(Runtime runtime);
	}

	public void testProfile(String name, GetProfile gp) {
		Runtime runtime = BerkeliumFactory.getInstance().createRuntime();
		assertChangedAndPush();
		{
			Profile profile = gp.getProfile(runtime);
			assertChangedAndPush();
			{
				Assert.assertEquals(name, profile.getApplicationName());
			}
			profile.dispose();
			popAndAssert();
		}
		runtime.dispose();
		popAndAssert();
	}

	@Test
	public void testCreateTemporaryProfile() {
		testProfile("berkelium", new GetProfile() {
			@Override
			public Profile getProfile(Runtime runtime) {
				return runtime.createTemporaryProfile();
			}
		});
	}

	@Test
	public void testGetChromeProfile() {
		testProfile("Google Chrome", new GetProfile() {
			@Override
			public Profile getProfile(Runtime runtime) {
				return runtime.getChromeProfile();
			}
		});
	}

	@Test
	public void testGetChromiumProfile() {
		testProfile("Chromium", new GetProfile() {
			@Override
			public Profile getProfile(Runtime runtime) {
				return runtime.getChromiumProfile();
			}
		});
	}

	@Test
	public void testGetBerkeliumProfile() {
		testProfile("berkelium", new GetProfile() {
			@Override
			public Profile getProfile(Runtime runtime) {
				return runtime.forProfile("berkelium");
			}
		});
	}
}
