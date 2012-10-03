package org.berkelium.java.api;

import java.nio.Buffer;

public interface WindowDelegate {
	void onWindowClosed(Window win);

	/*
	void onResize(Window win, int width, int height);

	void onPaintDone(Window win, Rect rect);

	void onAddressBarChanged(Window win, String newURL);

	void onStartLoading(Window win, String newURL);

	void onLoad(Window win);

	void onCrashedWorker(Window win);

	void onCrashedPlugin(Window win, String pluginName);

	void onProvisionalLoadError(Window win, String url, int errorCode, boolean isMainFrame);

	void onConsoleMessage(Window win, String message, String sourceId, int line_no);

	void onScriptAlert(Window win, String message, String defaultValue, String url, int flags, boolean success[], String value[]);

	void freeLastScriptAlert(String lastValue);

	boolean onNavigationRequested(Window win, String newUrl, String referrer, boolean isNewWindow, boolean cancelDefaultAction[]);

	void onLoadingStateChanged(Window win, boolean isLoading);

	void onTitleChanged(Window win, String title);

	void onTooltipChanged(Window win, String text);

	void onCrashed(Window win);

	void onUnresponsive(Window win);

	void onResponsive(Window win);

	void onExternalHost(Window win, String message, String origin, String target);

	void onCreatedWindow(Window win, Window newWindow, Rect initialRect);
	*/

	void onPaint(Window win, Buffer sourceBuffer, Rect sourceBufferRect, Rect copyRects[], int dx, int dy,
			Rect scrollRect);

/*
	void onJavascriptCallback(Window win, String url, String funcName);

	void onRunFileChooser(Window win, int mode, String title, String defaultFile);

	void onResizeRequested(Window win, int x, int y, int width, int height);
	*/
}
