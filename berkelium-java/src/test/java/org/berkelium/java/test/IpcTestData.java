package org.berkelium.java.test;

public class IpcTestData {
	public final static byte[] onPaint = {
		// @formatter:off
		0x00, 0x00, 0x00, 0x0b,		// Block Size = 11
		0x00, 0x13,					// Command: onPaint
		0x01,						// 1x copyRect
		0x00, 0x00,	0x00, 0x00,		// x = 0, y = 0
		0x03, 0x20, 0x02, 0x58		// w = 800, h = 600
		// @formatter:on
	};
}
