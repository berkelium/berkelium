package org.berkelium.java.test;

import java.io.IOException;
import java.util.concurrent.atomic.AtomicBoolean;

import junit.framework.Assert;

import org.berkelium.java.api.Rect;
import org.berkelium.java.api.Window;
import org.berkelium.java.api.WindowAdapter;
import org.berkelium.java.api.WindowDelegate;
import org.berkelium.java.impl.WindowDelegateProcessor;
import org.junit.Test;

public class WindowDelegateProcessorTest {
	@Test
	public void test() throws IOException {
		final AtomicBoolean test = new AtomicBoolean();
		WindowDelegate delegate = new WindowAdapter() {
			@Override
			public void onPaint(Window win, byte[] sourceBuffer, Rect sourceBufferRect, Rect[] copyRects, int dx,
					int dy, Rect scrollRect) {
				test.set(true);
				Assert.assertEquals(1, copyRects.length);
				Assert.assertEquals(0, copyRects[0].x);
				Assert.assertEquals(0, copyRects[0].y);
				Assert.assertEquals(800, copyRects[0].w);
				Assert.assertEquals(600, copyRects[0].h);
			}
		};
		WindowDelegateProcessor subject = new WindowDelegateProcessor(null, delegate);

		subject.process(IpcTestData.onPaint, 0, IpcTestData.onPaint.length);

		Assert.assertEquals(0, subject.available());

		Assert.assertTrue("onPaint not called!", test.get());
	}
}
