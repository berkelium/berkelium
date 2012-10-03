package org.berkelium.java.awt;

import java.awt.Graphics;
import java.awt.image.BufferedImage;

import javax.swing.JPanel;

import org.berkelium.java.api.Rect;
import org.berkelium.java.api.Window;

public class JBerkeliumPanel extends JPanel {
	private static final long serialVersionUID = 1L;
	private final BufferedImageAdapter bia = new BufferedImageAdapter() {
		@Override
		public void onPaintDone(Window win, Rect rect) {
			repaint(rect.left(), rect.top(), rect.right(), rect.bottom());
		}
	};

	public JBerkeliumPanel(Window win) {
		win.addDelegate(bia);
	}

	@Override
	public void paint(Graphics g) {
		BufferedImage img = bia.getImage();
		if(img != null) {
			// do not allow updates to the image while we draw it
			synchronized(bia) {
				g.drawImage(img, 0, 0, null);
			}
		}
	}
}
