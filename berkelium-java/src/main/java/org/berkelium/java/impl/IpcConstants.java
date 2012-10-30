package org.berkelium.java.impl;

public interface IpcConstants {
	public static interface Main {
		/*
		final static int TERMINATE = 0;
		final static int CREATE_WINDOW = 1;
		final static int CLOSE_WINDOW = 2;
		*/
	};

	public static interface Window {
		final static int DESTROY = 0;
		final static int NAVIGATE_TO = 1;
		/*
		final static int TRANSPARENT = 1;
		final static int FOCUS = 2;
		final static int UNFOCUS = 3;
		final static int MOUSE_MOVED = 4;
		final static int MOUSE_BUTTON = 5;
		final static int MOUSE_WHEEL = 6;
		final static int TEXT_EVENT = 7;
		final static int KEY_EVENT = 8;
		final static int ADJUST_ZOOM = 9;
		final static int RESIZE = 10;
		final static int EXECUTE_JAVASCRIPT = 11;
		final static int INSERT_CSS = 12;
		final static int REFRESH = 14;
		final static int STOP = 15;
		final static int GO_BACK = 16;
		final static int RGO_FORWARD = 17;
		final static int CAN_GO_BACK = 18;
		final static int CAN_GO_FORWARD = 19;
		final static int CUT = 20;
		final static int COPY = 21;
		final static int PASTE = 22;
		final static int UNDO = 23;
		final static int REDO = 24;
		final static int DEL = 25;
		final static int SELECT_ALL = 26;
		final static int FILES_SELECTED = 27;
		final static int ADD_EVAL_ON_START_LOADING = 28;
		final static int CLEAR_START_LOADING = 29;
		*/
	}

	public interface Delegate {
		final static int onPaint = 0;
		final static int onTitleChanged = 1;
		/*
		final static int onPaintDone = 0;
		final static int onAddressBarChanged = 1;
		final static int onStartLoading = 2;
		final static int onLoad = 3;
		final static int onCrashedWorker = 4;
		final static int onCrashedPlugin = 5;
		final static int onProvisionalLoadError = 6;
		final static int onConsoleMessage = 7;
		final static int onScriptAlert = 8;
		final static int freeLastScriptAlert = 9;
		final static int onNavigationRequested = 10;
		final static int onLoadingStateChanged = 11;
		final static int onTooltipChanged = 13;
		final static int onCrashed = 14;
		final static int onUnresponsive = 15;
		final static int onResponsive = 16;
		final static int onExternalHost = 17;
		final static int onCreatedWindow = 18;
		final static int onJavascriptCallback = 20;
		final static int onRunFileChooser = 21;
		*/
	}
}
