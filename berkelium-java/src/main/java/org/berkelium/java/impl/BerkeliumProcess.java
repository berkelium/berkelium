package org.berkelium.java.impl;

import java.io.IOException;
import java.io.InputStreamReader;

import org.berkelium.java.api.Berkelium;

public class BerkeliumProcess {
	private final Berkelium runtime;
	private final Process process;
	private final InputStreamReader outSr;
	private final InputStreamReader errSr;
	private final StringBuilder outSb = new StringBuilder();
	private final StringBuilder errSb = new StringBuilder();

	public BerkeliumProcess(int port, Berkelium runtime) {
		this.runtime = runtime;
		try {
			process = Runtime.getRuntime().exec("../berkelium/berkelium --berkelium=127.0.0.1:" + port);
			outSr = new InputStreamReader(process.getInputStream());
			errSr = new InputStreamReader(process.getErrorStream());
		} catch (IOException e) {
			throw new RuntimeException(e);
		}
	}

	public void update() {
		pull(outSr, outSb);
		pull(errSr, errSb);
	}

	int x = 0;

	private void pull(InputStreamReader sr, StringBuilder sb) {
		try {
			while(sr.ready()) {
				int c = sr.read();
				if(c == '\n') {
					x++;
					if(x >= 5) {
						process.getOutputStream().close();
						process.destroy();
					}
					runtime.log(sb.toString());
					sb.setLength(0);
					continue;
				}
				sb.append((char)c);
			}
		} catch (IOException e) {
			runtime.handleThrowable(e);
		}
	}

	public void destroy() {
		process.destroy();
		update();
	}
}
