package org.berkelium.java.api;

public class Rect {

	public final int x;
	public final int y;
	public final int w;
	public final int h;

	public Rect(int x, int y, int w, int h) {
		this.x = x;
		this.y = y;
		this.w = w;
		this.h = h;
	}

	public int left() {
		return x;
	}

	public int top() {
		return y;
	}

	public int right() {
		return x + w;
	}

	public int bottom() {
		return y + h;
	}

	public int width() {
		return w;
	}

	public int height() {
		return h;
	}

	public boolean isEmpty() {
		return w == 0 && h == 0;
	}

	public Rect translate(int i, int j) {
		return new Rect(x + i, y + j, w, h);
	}

	public Rect intersect(Rect rect) {
		int rx = Math.max(left(), rect.left());
		int ry = Math.max(top(), rect.top());
		int rr = Math.min(right(), rect.right());
		int rb = Math.min(bottom(), rect.bottom());
		if (rx >= rr || ry >= rb)
			rx = ry = rr = rb = 0; // non-intersecting
		return new Rect(rx, ry, rr - rx, rb - ry);
	}

	@Override
	public String toString() {
		return x + "x" + y + "/" + w + "x" + h;
	}

	public Rect grow(Rect other) {
		if (w == 0 || h == 0) {
			return new Rect(other.x, other.y, other.w, other.h);
		} else {
			// TODO junit test
			int left = Math.min(left(), other.left());
			int top = Math.min(top(), other.top());
			int right = Math.max(right(), other.right());
			int bottom = Math.max(bottom(), other.bottom());
			return new Rect(left, top, right - left, bottom - top);
		}
	}
	
	@Override
	public boolean equals(Object obj) {
		if(obj instanceof Rect) {
			Rect r = (Rect) obj;
			return r.x == x && r.y == y && r.w == w && r.h == h;
		}
		return false;
	}
}
