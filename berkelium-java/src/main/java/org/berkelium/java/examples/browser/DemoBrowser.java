package org.berkelium.java.examples.browser;

import java.awt.Dimension;

import javax.swing.JFrame;
import javax.swing.JOptionPane;

import org.berkelium.java.api.Berkelium;
import org.berkelium.java.api.BerkeliumAdapter;
import org.berkelium.java.api.BerkeliumDelegate;
import org.berkelium.java.api.BerkeliumFactory;
import org.berkelium.java.api.Window;
import org.berkelium.java.api.WindowAdapter;
import org.berkelium.java.api.WindowDelegate;
import org.berkelium.java.awt.JBerkeliumAdapter;
import org.berkelium.java.awt.JBerkeliumPanel;
import org.berkelium.java.awt.JBerkeliumWindowAdapter;

public class DemoBrowser extends JFrame {
	private static final String TITLE = "Berkelium Demo Browser";

	private final static long serialVersionUID = -1;

	private final Berkelium runtime = BerkeliumFactory.createTemporaryInstance();
	private final Window win = runtime.createWindow();
	private final JBerkeliumPanel panel = new JBerkeliumPanel(win);

	private final BerkeliumDelegate delegate = new JBerkeliumAdapter(new BerkeliumAdapter() {
		@Override
		public void onCrashed(Berkelium runtime) {
			JOptionPane.showMessageDialog(DemoBrowser.this, "Berkelium crashed!", "Error", JOptionPane.ERROR_MESSAGE);
			dispose();
		}

		@Override
		public void onClosed(Berkelium runtime) {
			JOptionPane.showMessageDialog(DemoBrowser.this, "Berkelium closed!", "Info",
				JOptionPane.INFORMATION_MESSAGE);
			dispose();
		}
	});
	private final WindowDelegate winDelegate = new WindowAdapter() {
		@Override
		public void onWindowClosed(Window win) {
			dispose();
		}

		@Override
		public void onTitleChanged(Window win, String title) {
			setTitle(title + " - " + TITLE);
		}
	};

	public DemoBrowser() {
		runtime.addDelegate(delegate);

		setTitle(TITLE);
		setSize(new Dimension(1024, 768));
		setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		add(panel);
		setVisible(true);

		win.addDelegate(new JBerkeliumWindowAdapter(winDelegate).getProxy());
		win.navigateTo("http://www.youtube.com/");
	}

	public static void main(String[] args) {
		new DemoBrowser();
	}
}
