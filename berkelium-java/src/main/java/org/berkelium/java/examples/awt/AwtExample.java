package org.berkelium.java.examples.awt;

import java.awt.Dimension;

import javax.swing.JFrame;
import javax.swing.JOptionPane;

import org.berkelium.java.api.Berkelium;
import org.berkelium.java.api.BerkeliumAdapter;
import org.berkelium.java.api.BerkeliumDelegate;
import org.berkelium.java.api.BerkeliumFactory;
import org.berkelium.java.api.DebugDelegate;
import org.berkelium.java.api.Window;
import org.berkelium.java.api.WindowAdapter;
import org.berkelium.java.api.WindowDelegate;
import org.berkelium.java.awt.JBerkeliumAdapter;
import org.berkelium.java.awt.JBerkeliumPanel;
import org.berkelium.java.awt.JBerkeliumWindowAdapter;

public class AwtExample extends JFrame {
	private final static long serialVersionUID = -1;

	private final int WIDTH = 640;
	private final int HEIGHT = 480;

	private final Berkelium runtime = BerkeliumFactory.createTemporaryInstance();
	private final Window win = runtime.createWindow();
	private final JBerkeliumPanel panel = new JBerkeliumPanel(win);
	private final BerkeliumDelegate delegate = new JBerkeliumAdapter(new BerkeliumAdapter() {
		@Override
		public void onCrashed(Berkelium runtime) {
			JOptionPane.showMessageDialog(AwtExample.this, "Berkelium crashed!", "Error", JOptionPane.ERROR_MESSAGE);
			dispose();
		}

		@Override
		public void onClosed(Berkelium runtime) {
			JOptionPane
					.showMessageDialog(AwtExample.this, "Berkelium closed!", "Info", JOptionPane.INFORMATION_MESSAGE);
			dispose();
		}
	});
	private final WindowDelegate winDelegate = new WindowAdapter() {
		@Override
		public void onWindowClosed(Window win) {
			dispose();
		}
	};

	public AwtExample() {
		runtime.addDelegate(delegate);

		setTitle("AwtExample");
		setSize(new Dimension(WIDTH, HEIGHT));
		setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		add(panel);
		setVisible(true);

		win.addDelegate(new DebugDelegate(runtime).getProxy());
		win.addDelegate(new JBerkeliumWindowAdapter(winDelegate).getProxy());
		win.navigateTo("http://www.youtube.com/");
	}

	public static void main(String[] args) {
		new AwtExample();
	}
}
