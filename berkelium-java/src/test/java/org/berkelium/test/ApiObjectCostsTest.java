package org.berkelium.test;

import java.util.HashMap;
import java.util.HashSet;
import java.util.Map;
import java.util.Set;
import java.util.StringTokenizer;


import org.berkelium.api.HostExecutable;
import org.berkelium.api.Instance;
import org.berkelium.api.Profile;
import org.berkelium.api.Runtime;
import org.berkelium.impl.TestHelper;
import org.junit.After;
import org.junit.Before;
import org.junit.Test;
import org.junit.Assert;

public class ApiObjectCostsTest {
	static class Costs {
		public final Map<String, Integer> costs;

		public Costs() {
			this(TestHelper.getNativeObjectCount());
		}

		public Costs(String costs) {
			this.costs = new HashMap<String, Integer>();
			StringTokenizer st = new StringTokenizer(costs, "\n");
			while(st.hasMoreTokens()) {
				StringTokenizer st2 = new StringTokenizer(st.nextToken(), " ");
				String key = st2.nextToken();
				String value = st2.nextToken();
				this.costs.put(key, Integer.valueOf(value));
			}
		}

		public Costs(String all, String linux, String win, String mac) {
			this(all + getForCurrentSystem(linux, win, mac));
		}

		private static String getForCurrentSystem(String linux, String win, String mac) {
			String name = System.getProperty("os.name");
			if(name.contains("Windows")) {
				return win;
			} else 	if(name.contains("Linux")) {
				return linux;
			}
			return mac;
		}

		public Costs diff(Costs other) {
			Costs ret = new Costs("");
			Set<String> keys = new HashSet<String>(costs.keySet());
			keys.addAll(other.costs.keySet());
			for(String key : keys) {
				Integer a = costs.get(key);
				Integer b = other.costs.get(key);
				if(a == null) a = 0;
				if(b == null) b = 0;
				int d = a - b;
				if(d < 0) {
					Assert.fail();
				} else if(d != 0) {
					ret.costs.put(key, a - b);
				}
			}
			return ret;
		}

		public void assertSame(Costs other) {
			Assert.assertEquals(0, diff(other).costs.size());
		}

		public void assertCount(String key, int expected) {
			Integer value = costs.get(key);
			if(value == null) {
				costs.put(key, -expected);
			} else if(expected == value) {
				costs.remove(key);
			} else {
				costs.put(key, value - expected);
			}
		}

		public void assertEmpty() {
			Assert.assertTrue(costs.isEmpty());
		}

		public void assertCount(Costs check, int multiply) {
			for(String key : check.costs.keySet()) {
				assertCount(key, check.costs.get(key) * multiply);
			}
		}
	}

	private final Costs CHECK_MANAGER = new Costs("TypeManager 8\nManagerImpl 1\n",
			// Linux
			"",
			// Windows
			"",
			// Mac
			"");
	private final Costs CHECK_RUNTIME = new Costs("Runtime 1\nRuntimeImpl 1\n",
			// Linux
			"",
			// Windows
			"",
			// Mac
			"");
	private final Costs CHECK_CHANNEL = new Costs("Channel 1\nChannelImpl 1\n",
			// Linux
			"",
			// Windows
			"",
			// Mac
			"");
	private final Costs CHECK_PROFILE = new Costs("Profile 1\nProfileImpl 1\n",
			// Linux
			"",
			// Windows
			"",
			// Mac
			"");
	private final Costs CHECK_PROCESS = new Costs("Process 1\n",
			// Linux
			"ProcessLinux 1\nConsoleRedirector 2\nLinkLinuxImpl 2\nPipePosixImpl 2\nLinkCallback 2\n",
			// Windows
			"",
			// Mac
			"");
	private final Costs CHECK_INSTANCE = new Costs("Instance 1\nInstanceImpl 1\n",
			// Linux
			"",
			// Windows
			"",
			// Mac
			"");
	private final Costs CHECK_LOGGER = new Costs("Logger 1\nLoggerImpl 1\nLoggerStream 1\nLoggerStreamBuffer 1\n",
			// Linux
			"",
			// Windows
			"",
			// Mac
			"");
	private final Costs CHECK_EXECUTABLE = new Costs("HostExecutable 1\nHostExecutableImpl 1",
			// Linux
			"",
			// Windows
			"",
			// Mac
			"");
	private final Costs CHECK_LINKGROUP = new Costs("LinkGroup 1\nLinkGroupImpl 1\n",
			// Linux
			"",
			// Windows
			"",
			// Mac
			"");
	private final Costs CHECK_CHANNELGROUP = new Costs("ChannelGroup 1\nChannelGroupImpl 1\nLinkCallback 2\n",
			// Linux
			"LinkLinuxImpl 1\nPipePosixImpl 4\n",
			// Windows
			"",
			// Mac
			"");

	@Before
	@After
	public void assertNoNativeObjects() {
		Assert.assertEquals("", TestHelper.getNativeObjectCount());
	}

	@Test
	public void Runtime_createRuntime() {
		Costs before = new Costs();
		Runtime runtime = TestHelper.createRuntime();
		Costs after = new Costs();
		runtime.dispose();
		before.assertSame(new Costs());
		Costs diff = after.diff(before);

		diff.assertCount(CHECK_MANAGER, 1);
		diff.assertCount(CHECK_LOGGER, 1);
		diff.assertCount(CHECK_RUNTIME, 1);
		diff.assertCount(CHECK_LINKGROUP, 1);

		diff.assertCount("RuntimeLogDelegate", 1);

		diff.assertEmpty();
	}

	@Test
	public void Runtime_createTemporaryProfile() {
		Runtime runtime = TestHelper.createRuntime();
		Costs before = new Costs();
		Profile profile = runtime.createTemporaryProfile();
		Costs after = new Costs();
		profile.dispose();
		before.assertSame(new Costs());
		Costs diff = after.diff(before);
		runtime.dispose();

		diff.assertCount(CHECK_LOGGER, 1);
		diff.assertCount(CHECK_PROFILE, 1);
		diff.assertEmpty();
	}

	@Test
	public void Runtime_getChromeProfile() {
		Runtime runtime = TestHelper.createRuntime();
		Costs before = new Costs();
		Profile profile = runtime.getChromeProfile();
		Costs after = new Costs();
		profile.dispose();
		before.assertSame(new Costs());
		Costs diff = after.diff(before);
		runtime.dispose();

		diff.assertCount(CHECK_LOGGER, 1);
		diff.assertCount(CHECK_PROFILE, 1);
		diff.assertEmpty();
	}

	@Test
	public void Runtime_getChromiumProfile() {
		Runtime runtime = TestHelper.createRuntime();
		Costs before = new Costs();
		Profile profile = runtime.getChromiumProfile();
		Costs after = new Costs();
		profile.dispose();
		before.assertSame(new Costs());
		Costs diff = after.diff(before);
		runtime.dispose();

		diff.assertCount(CHECK_LOGGER, 1);
		diff.assertCount(CHECK_PROFILE, 1);
		diff.assertEmpty();
	}

	@Test
	public void Runtime_forSystemInstalled() {
		Runtime runtime = TestHelper.createRuntime();
		Costs before = new Costs();
		HostExecutable executable = runtime.forSystemInstalled();
		Costs after = new Costs();
		executable.dispose();
		before.assertSame(new Costs());
		Costs diff = after.diff(before);
		runtime.dispose();

		diff.assertCount(CHECK_LOGGER, 1);
		diff.assertCount(CHECK_EXECUTABLE, 1);
		diff.assertEmpty();
	}

	@Test
	public void Runtime_open() {
		Runtime runtime = TestHelper.createRuntime();
		HostExecutable executable = runtime.forSystemInstalled();
		Profile profile = runtime.createTemporaryProfile();
		Costs before = new Costs();
		Instance instance = runtime.open(executable, profile);
		Costs after = new Costs();
		instance.dispose();
		before.assertSame(new Costs());
		Costs diff = after.diff(before);
		profile.dispose();
		executable.dispose();
		runtime.dispose();

		diff.assertCount(CHECK_LOGGER, 2);
		diff.assertCount(CHECK_PROCESS, 1);
		diff.assertCount(CHECK_INSTANCE, 1);
		diff.assertCount(CHECK_CHANNELGROUP, 1);
		diff.assertCount(CHECK_CHANNEL, 4);
		diff.assertCount("ChannelCallback", 2);
		diff.assertEmpty();
	}
}
