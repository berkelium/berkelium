package org.berkelium.test;

import org.berkelium.api.TabDelegate;
import org.junit.Test;
import org.junit.runner.RunWith;
import org.mockito.Mock;
import org.mockito.runners.MockitoJUnitRunner;

@RunWith(MockitoJUnitRunner.class)
public class TabDelegateTest extends AbstractTabTest {
	@Mock
	private TabDelegate delegate;

	@Override
	public void before() {
		super.before();
		tab.addTabDelegate(delegate);
	}

	@Test
	public void testOnReady() {
		// TODO
		runtime.update(5000);
	}
}
