package org.berkelium.java.impl;

import java.io.DataInputStream;
import java.io.IOException;

import org.berkelium.java.api.Rect;
import org.berkelium.java.api.WindowDelegate;
import org.berkelium.java.impl.IpcConstants.Delegate;

public class WindowDelegateProcessor extends BerkeliumProcessor {
	private final WindowDelegate delegate;

	public WindowDelegateProcessor(WindowDelegate delegate) {
		this.delegate = delegate;
	}

	@Override
	protected void process(DataInputStream data) throws IOException {
		short command = data.readShort();

		if(command == Delegate.onPaint.ordinal()) {
			System.err.println("recv: onPaint!");
			int to = data.readByte();
			Rect[] copyRects = new Rect[to];
			for(int i = 0; i < to; i++) {
				int x = data.readShort();
				int y = data.readShort();
				int w = data.readShort();
				int h = data.readShort();
				copyRects[i] = new Rect(x, y, w, h);
				System.err.println(copyRects[i]);
			}
			delegate.onPaint(null, null, null, copyRects, 0, 0, null);
		} else if(command == Delegate.onTitleChanged.ordinal()) {
			System.err.printf("recv: onTitleChanged('%s')!\n", data.readUTF());
		} else {
			System.err.printf("recv: unknown ipc code %d!\n", command);
		}
	}
}
