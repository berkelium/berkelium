package org.berkelium.test;

import java.util.LinkedList;

import org.berkelium.impl.TestHelper;
import org.junit.After;
import org.junit.Assert;
import org.junit.Before;

public abstract class AbstractTest {
	private LinkedList<Integer> objectCount = new LinkedList<Integer>();

	@Before
	public void before() {
		pushObjectCount();
	}

	@After
	public void after() {
		assertUnchanged();
		Assert.assertEquals(1, objectCount.size());
	}

	protected void pushObjectCount() {
		objectCount.addLast(TestHelper.getObjectCount());
	}

	protected void popAndAssert() {
		objectCount.removeLast();
		assertUnchanged();
	}

	protected void assertChanged() {
		Assert.assertNotEquals(objectCount.peekLast(), TestHelper.getObjectCount());
	}

	protected void assertUnchanged() {
		Assert.assertEquals(objectCount.peekLast(), TestHelper.getObjectCount());
	}

	protected void assertChangedAndPush() {
		assertChanged();
		pushObjectCount();
	}
}
