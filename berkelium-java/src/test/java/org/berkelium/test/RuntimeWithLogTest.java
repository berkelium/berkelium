package org.berkelium.test;

import org.berkelium.api.BerkeliumFactory;
import org.berkelium.api.LogDelegate;
import org.berkelium.api.Logger;
import org.berkelium.api.Runtime;
import org.junit.Test;
import org.junit.runner.RunWith;
import org.mockito.InOrder;
import org.mockito.Mock;
import org.mockito.Mockito;
import org.mockito.runners.MockitoJUnitRunner;

@RunWith(MockitoJUnitRunner.class)
public class RuntimeWithLogTest extends AbstractRuntimeTest {
	@Mock
	private LogDelegate log;

	@Override
	protected Runtime createRuntime() {
		return BerkeliumFactory.getInstance().createRuntimeWithLog(log);
	}

	@Test
	public void testRuntimeWithLogDispose() {
		final String clazz = RuntimeWithLogTest.class.getName();
		final String name = "name...";
		final String debug = "debug: Hello World!";
		final String info = "info: Hello World!";
		final String warn = "warn: Hello World!";
		final String error = "error: Hello World!";

		Logger logger = runtime.getLogger(clazz, name);
		assertChangedAndPush();
		{
			logger.debug(debug);
			logger.info(info);
			logger.warn(warn);
			logger.error(error);
			assertUnchanged();
		}
		logger.dispose();
		popAndAssert();

		// TODO
		// log(runtime, LogSource.Lib, LogType.TODO, ...);

		InOrder inOrder = Mockito.inOrder(log);
		inOrder.verify(log).log(null, null, null, clazz, name, debug);
		inOrder.verify(log).log(null, null, null, clazz, name, info);
		inOrder.verify(log).log(null, null, null, clazz, name, warn);
		inOrder.verify(log).log(null, null, null, clazz, name, error);
	}
}
