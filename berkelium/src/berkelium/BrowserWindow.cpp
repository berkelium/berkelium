// Copyright (c) 2012 The Berkelium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "Berkelium.hpp"

#include "chrome/browser/ui/browser.h"
#include "chrome/browser/ui/browser_list.h"
#include "chrome/browser/ui/browser_window.h"
#include "chrome/browser/ui/tab_contents/tab_contents.h"
#include "chrome/browser/ui/tabs/tab_strip_model.h"
#include "chrome/browser/ui/tabs/tab_strip_model_observer.h"
#include "content/public/browser/web_contents.h"
#include "content/public/browser/web_contents_view.h"

#include "ui/gfx/rect.h"

#define X //fprintf(stderr, "BerkeliumBrowserWindow::%s\n", __func__)

class BerkeliumBrowserWindow : public BrowserWindow, public TabStripModelObserver {
public:
	BerkeliumBrowserWindow(Browser* browser) : browser(browser) {
		browser->tab_strip_model()->AddObserver(this);
	}

	virtual ~BerkeliumBrowserWindow() {
	}

private:
	Browser* browser;

	virtual void ActiveTabChanged(TabContents* old_contents, TabContents* new_contents, int index, bool user_gesture) {
		fprintf(stderr, "BerkeliumBrowserWindow::%s %p %p %d %s\n", __func__, (void*)old_contents, (void*)new_contents, index, user_gesture ? "true" : "false");
		if(new_contents != NULL) {
			new_contents->web_contents()->GetView()->SizeContents(gfx::Size(800, 600));
		}
	}

	virtual bool IsActive() const {X;return true;}
	virtual bool IsMaximized() const {X;return false;}
	virtual bool IsMinimized() const {X;return false;}
	virtual bool IsFullscreen() const {X;return false;}
	virtual gfx::Rect GetRestoredBounds() const {X;return gfx::Rect(640, 480);}
	virtual gfx::Rect GetBounds() const {X;return gfx::Rect(640, 480);}
	virtual void Show() {X;
	BrowserList::SetLastActive(browser);
	browser->OnWindowDidShow();
	//browser->GetActiveTabContents()
	}
	virtual void ShowInactive() {X;}
	virtual void Close() {X;}
	virtual void Activate() {X;}
	virtual void Deactivate() {X;}
	virtual void Maximize() {X;}
	virtual void Minimize() {X;}
	virtual void Restore() {X;}
	virtual void SetBounds(const gfx::Rect&) {X;}
	virtual void FlashFrame(bool) {X;}
	virtual bool IsAlwaysOnTop() const {X;return false;}
	virtual BrowserWindowTesting* GetBrowserWindowTesting() {X;return NULL;}
	virtual StatusBubble* GetStatusBubble() {X;return NULL;}
	virtual void ToolbarSizeChanged(bool) {X;}
	virtual void UpdateTitleBar() {X;}
	virtual void BookmarkBarStateChanged(BookmarkBar::AnimateChangeType) {X;}
	virtual void UpdateDevTools() {X;}
	virtual void SetDevToolsDockSide(DevToolsDockSide) {X;}
	virtual void UpdateLoadingAnimations(bool) {X;}
	virtual void SetStarredState(bool) {X;}
	virtual void SetZoomIconState(ZoomController::ZoomIconState) {X;}
	virtual void SetZoomIconTooltipPercent(int) {X;}
	virtual void ShowZoomBubble(int) {X;}
	virtual void EnterFullscreen(const GURL&, FullscreenExitBubbleType) {X;}
	virtual void ExitFullscreen() {X;}
	virtual void UpdateFullscreenExitBubbleContent(const GURL&, FullscreenExitBubbleType) {X;}
	virtual bool IsFullscreenBubbleVisible() const {X;return false;}
	virtual LocationBar* GetLocationBar() const {X;return NULL;}
	virtual void SetFocusToLocationBar(bool) {X;}
	virtual void UpdateReloadStopState(bool, bool) {X;}
	virtual void UpdateToolbar(TabContents*, bool) {X;}
	virtual void FocusToolbar() {X;}
	virtual void FocusAppMenu() {X;}
	virtual void FocusBookmarksToolbar() {X;}
	virtual void RotatePaneFocus(bool) {X;}
	virtual bool IsBookmarkBarVisible() const {X;return false;}
	virtual bool IsBookmarkBarAnimating() const {X;return false;}
	virtual bool IsTabStripEditable() const {X;return false;}
	virtual bool IsToolbarVisible() const {X;return false;}
	virtual gfx::Rect GetRootWindowResizerRect() const {X;return gfx::Rect();}
	virtual bool IsPanel() const {X;return false;}
	virtual void ConfirmAddSearchProvider(TemplateURL*, Profile*) {X;}
	virtual void ToggleBookmarkBar() {X;}
	virtual void ShowAboutChromeDialog() {X;}
	virtual void ShowUpdateChromeDialog() {X;}
	virtual void ShowTaskManager() {X;}
	virtual void ShowBackgroundPages() {X;}
	virtual void ShowBookmarkBubble(const GURL&, bool) {X;}
	virtual void ShowChromeToMobileBubble() {X;}
	virtual bool IsDownloadShelfVisible() const {X;return false;}
	virtual DownloadShelf* GetDownloadShelf() {X;return NULL;}
	virtual void ConfirmBrowserCloseWithPendingDownloads() {X;}
	virtual void UserChangedTheme() {X;}
	virtual int GetExtraRenderViewHeight() const {X;return 0;}
	virtual void WebContentsFocused(content::WebContents*) {X;}
	virtual void ShowPageInfo(Profile*, const GURL&, const content::SSLStatus&, bool) {X;}
	virtual void ShowWebsiteSettings(Profile*, TabContents*, const GURL&, const content::SSLStatus&, bool) {X;}
	virtual void ShowAppMenu() {X;}
	virtual bool PreHandleKeyboardEvent(const content::NativeWebKeyboardEvent&, bool*) {X;return false;}
	virtual void HandleKeyboardEvent(const content::NativeWebKeyboardEvent&) {X;}
	virtual void ShowCreateWebAppShortcutsDialog(TabContents*) {X;}
	virtual void ShowCreateChromeAppShortcutsDialog(Profile*, const extensions::Extension*) {X;}
	virtual void Cut() {X;}
	virtual void Copy() {X;}
	virtual void Paste() {X;}
	virtual void ShowInstant(TabContents*) {X;fprintf(stderr, "ShowInstant\n");}
	virtual void HideInstant() {X;fprintf(stderr, "HideInstant\n");}
	virtual gfx::Rect GetInstantBounds() {X;return gfx::Rect(640, 480);}
	virtual WindowOpenDisposition GetDispositionForPopupBounds(const gfx::Rect&) {X;return WindowOpenDisposition();}
	virtual FindBar* CreateFindBar() {X;return NULL;}
	virtual void ShowAvatarBubble(content::WebContents*, const gfx::Rect&) {X;}
	virtual void ShowAvatarBubbleFromAvatarButton() {X;}
	virtual void DestroyBrowser() {X;}
	virtual gfx::NativeWindow GetNativeWindow() {X;fprintf(stderr, "GetNativeBrowser\n");return NULL;}
};

BrowserWindow* BerkeliumCreateBrowserWindow(Browser* browser);

// static
BrowserWindow* BrowserWindow::CreateBrowserWindow(Browser* browser) {
	BrowserWindow* ret;

	fprintf(stderr, "CreateBrowserWindow start\n");
	if(Berkelium::Berkelium::isActive()) {
		ret = new BerkeliumBrowserWindow(browser);
	} else {
		ret = BerkeliumCreateBrowserWindow(browser);
	}
	fprintf(stderr, "CreateBrowserWindow done\n");
	return ret;
}
