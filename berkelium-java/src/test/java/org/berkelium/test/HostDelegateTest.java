package org.berkelium.test;

import org.berkelium.api.HostDelegate;
import org.berkelium.api.Instance;
import org.berkelium.api.Window;
import org.junit.Assert;
import org.junit.Test;

public class HostDelegateTest extends AbstractInstanceTest {
	static public class TestHostDelegate implements HostDelegate {
		public Instance instance;

		@Override
		public void onCrashed(Instance instance) {
			Assert.assertSame(this.instance, instance);
		}

		@Override
		public void onClosed(Instance instance) {
			Assert.assertSame(this.instance, instance);
		}

		@Override
		public void onPing(Instance instance) {
			Assert.assertSame(this.instance, instance);
		}
	}

	private TestHostDelegate delegate = new TestHostDelegate();

	@Override
	public void before() {
		super.before();
		delegate.instance = instance;
		instance.addHostDelegate(delegate);
	}

	@Override
	public void after() {
		delegate.instance = null;
		//instance.removeHostDelegate(delegate);
		super.after();
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
