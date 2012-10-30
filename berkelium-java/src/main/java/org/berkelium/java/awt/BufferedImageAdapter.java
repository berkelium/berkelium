package org.berkelium.java.awt;

import java.awt.image.BufferedImage;
import java.awt.image.WritableRaster;

import org.berkelium.java.api.Rect;
import org.berkelium.java.api.Window;
import org.berkelium.java.api.WindowAdapter;
import org.berkelium.java.api.WindowDelegate;

public class BufferedImageAdapter extends WindowAdapter {
	private BufferedImage img = new BufferedImage(800, 600, BufferedImage.TYPE_4BYTE_ABGR);
	private boolean needs_full_refresh = true;

	public BufferedImageAdapter() {
	}

	public BufferedImageAdapter(Window win) {
		win.addDelegate(this);
	}

/*
	public void onResize(Window win, int width, int height) {
		resize(width, height);
	}

	@Deprecated
	public void resize(int width, int height) {
		if(img != null && img.getWidth() == width && img.getHeight() == height) {
			return;
		}
		setImage(new BufferedImage(width, height, BufferedImage.TYPE_INT_ARGB));
		needs_full_refresh = true;
	}

	public void setImage(BufferedImage img) {
		this.img = img;
	}
 */

	public BufferedImage getImage() {
		/*
		 * if (needs_full_refresh) { return null; }
		 */
		return img;
	}

	@Override
	public void onPaint(Window win, byte[] bitmap_in, Rect bitmap_rect, Rect[] copy_rects, int dx, int dy,
			Rect scroll_rect) {
		// we have no image yet
		if(img == null || win == null)
			return;

		WindowDelegate d = win.getDelegate();
		if(d == null)
			return;

		/*
		Graphics g = img.getGraphics();
		for(Rect rect : copy_rects) {
			g.drawRect(rect.x, rect.y, rect.width(), rect.height());
			win.getDelegate().onPaintDone(win, rect);
		}
		*/

		try {
			synchronized(this) {
				if(needs_full_refresh) {
					// If we've reloaded the page and need a full update, ignore
					// updates
					// until a full one comes in. This handles out of date updates
					// due to
					// delays in event processing.
					if(handleFullUpdate(bitmap_in, bitmap_rect)) {
						needs_full_refresh = false;
					} else {
						return;
					}
				} else {
					// Now, we first handle scrolling. We need to do this first
					// since it
					// requires shifting existing data, some of which will be
					// overwritten
					// by the regular dirty rect update.
					if(dx != 0 || dy != 0) {
						handleScroll(dx, dy, scroll_rect);
					}
					handleCopyRects(bitmap_in, bitmap_rect, copy_rects);
				}
			}
			d.onPaintDone(win, bitmap_rect);
		} catch (ArrayIndexOutOfBoundsException ex) {
			needs_full_refresh = true;
		}
	}

	/**
	 * @param buf
	 * @param rect
	 * @return
	 */
	protected boolean handleFullUpdate(byte[] buf, Rect rect) {
		final int w = img.getWidth();
		final int h = img.getHeight();
		if(rect.left() == 0 || rect.top() == 0 || rect.right() == w || rect.bottom() == h) {
			final WritableRaster wr = img.getRaster();
			wr.setDataElements(0, 0, w, h, buf);
			return true;
		}
		return false;
	}

	private void handleScroll(int dx, int dy, Rect scroll_rect) {
		WritableRaster wr = img.getRaster();

		// scroll_rect contains the Rect we need to move
		// First we figure out where the data is moved to by translating it
		Rect scrolled_rect = scroll_rect.translate(-dx, -dy);
		// Next we figure out where they intersect, giving the scrolled region
		Rect scrolled_shared_rect = scroll_rect.intersect(scrolled_rect);
		// Only do scrolling if they have non-zero intersection
		if(scrolled_shared_rect.width() > 0 && scrolled_shared_rect.height() > 0) {
			Rect shared_rect = scrolled_shared_rect.translate(dx, dy);

			Object pixels = wr.getDataElements(scrolled_shared_rect.x, scrolled_shared_rect.y, shared_rect.w,
				shared_rect.h, null);

			wr.setDataElements(shared_rect.x, shared_rect.y, shared_rect.w, shared_rect.h, pixels);
		}
	}

	private void handleCopyRects(byte[] bitmap_in, Rect bitmap_rect, Rect[] copy_rects) {
		WritableRaster wr = img.getRaster();

		for(int i = 0; i < copy_rects.length; i++) {
			int wid = copy_rects[i].width();
			int hig = copy_rects[i].height();
			int top = copy_rects[i].top() - bitmap_rect.top();
			int left = copy_rects[i].left() - bitmap_rect.left();
			byte[] scroll_buffer = new byte[hig * wid * 4];
			for(int jj = 0; jj < hig; jj++) {
				memcpy(//
					scroll_buffer, //
					jj * wid, //
					bitmap_in, //
					left + (jj + top) * bitmap_rect.width(), //
					wid//
				);
			}

			// Finally, we perform the main update, just copying the rect that
			// is
			// marked as dirty but not from scrolled data.
			wr.setDataElements(copy_rects[i].left(), copy_rects[i].top(), wid, hig, scroll_buffer);
		}
	}

	private void memcpy(byte dest[], int destPos, byte src[], int srcPos, int length) {
		if(srcPos >= src.length) {
			return;
		}
		if(destPos >= dest.length) {
			return;
		}
		System.arraycopy(src, srcPos, dest, destPos, length);
	}
}
