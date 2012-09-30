package org.berkelium.java.api;

public interface Window {
/*
	WindowDelegate getDelegate();
*/
	void addDelegate(WindowDelegate delegate);

	void removeDelegate(WindowDelegate delegate);

/*
	int getId();

	void setTransparent(boolean istrans);

	void focus();

	void unfocus();

	void mouseMoved(int xPos, int yPos);

	void mouseButton(int buttonID, boolean down);

	void mouseWheel(int xScroll, int yScroll);

	void textEvent(String evt);

	void keyEvent(boolean pressed, int mods, int vk_code, int scancode);
*/
	void resize(int width, int height);

/*
	void adjustZoom(int mode);

	void executeJavascript(String javascript);

	void insertCSS(String css, String elementId);
*/
	boolean navigateTo(String url);

/*
	void refresh();

	void stop();

	void goBack();

	void goForward();

	boolean canGoBack();

	boolean canGoForward();

	void cut();

	void copy();

	void paste();

	void undo();

	void redo();

	void del();

	void selectAll();

	void filesSelected(String files[]);

	void addEvalOnStartLoading(String script);

	void clearStartLoading();

	void destroy();

	Berkelium getBerkelium();

	void bind(String name, Object target, String method, Class<?>... types);

	void call(String name, Object... arguments);

	void setHtml(String html);

	void setHtml(InputStream html);
*/
}
