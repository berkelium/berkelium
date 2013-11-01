package org.berkelium.test;

import org.berkelium.api.Rect;
import org.berkelium.api.Tab;
import org.berkelium.api.TabDelegate;
import org.junit.Assert;
import org.junit.Test;

public class TabDelegateTest extends AbstractTabTest {
	static public class TestTabDelegate implements TabDelegate {
		public String title;
		public int onReadyCount;
		public int onClosedCount;

		@Override
		public void onTitleChanged(Tab tab, String title) {
			this.title = title;
		}

		@Override
		public void onReady(Tab tab) {
			onReadyCount++;
		}

		@Override
		public void onPaintDone(Tab tab, Rect rect) {
			// TODO
		}

		@Override
		public void onPaint(Tab tab) {
			// TODO
		}

		@Override
		public void onClosed(Tab tab) {
			onClosedCount++;
		}
	};

	private TestTabDelegate delegate = new TestTabDelegate();

	@Override
	public void before() {
		super.before();
		tab.addTabDelegate(delegate);
	}

	@Test
	public void testOnReady() {
		for(int i = 0; i < 5000; i += 100) {
			runtime.update(100);
			if(delegate.onReadyCount != 0) {
				return;
			}
		}
		Assert.fail("TabDelegate.onReady() not called!");
	}
}
