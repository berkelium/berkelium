package org.berkelium.java.impl;

import java.io.DataInputStream;
import java.io.IOException;

import org.berkelium.java.api.Rect;
import org.berkelium.java.api.Window;
import org.berkelium.java.api.WindowDelegate;
import org.berkelium.java.impl.IpcConstants.Delegate;

public class WindowDelegateProcessor extends BerkeliumProcessor {
	private final Window win;
	private final WindowDelegate delegate;

	public WindowDelegateProcessor(Window win, WindowDelegate delegate) {
		this.win = win;
		this.delegate = delegate;
	}

	@Override
	protected void process(DataInputStream data) throws IOException {
		short command = data.readShort();

		// TODO: factory for Delegates
		if(command == Delegate.onPaint) {
			System.err.println("recv: onPaint!");
			int to = data.readByte();
			Rect[] copyRects = new Rect[to];
			for(int i = 0; i < to; i++) {
				copyRects[i] = new Rect(data);
			}
			Rect bitmap_rect = new Rect(data);
			int size = data.readInt();
			byte[] buf = new byte[size];
			data.read(buf, 0, size);
			delegate.onPaint(win, buf, bitmap_rect, copyRects, 0, 0, null);
		} else if(command == Delegate.onTitleChanged) {
			delegate.onTitleChanged(win, data.readUTF());
		} else {
			System.err.printf("recv: unknown ipc code %d!\n", command);
		}
	}
}
