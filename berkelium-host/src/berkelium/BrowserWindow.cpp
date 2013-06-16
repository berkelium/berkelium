// Copyright (c) 2012 The Berkelium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "BerkeliumHost.hpp"

#include "chrome/browser/ui/browser.h"
#include "chrome/browser/ui/browser_list.h"
#include "chrome/browser/ui/browser_window.h"
#include "chrome/browser/ui/tabs/tab_strip_model.h"
#include "chrome/browser/ui/tabs/tab_strip_model_observer.h"
#include "content/public/browser/web_contents.h"
#include "content/public/browser/web_contents_view.h"

#include "ui/gfx/rect.h"

#define X //fprintf(stderr, "BerkeliumBrowserWindow::%s\n", __func__)

class BerkeliumBrowserWindow : public BrowserWindow, public TabStripModelObserver {
public:
	BerkeliumBrowserWindow(Browser* browser) : browser(browser) {
		fprintf(stderr, "this: %p\n", this);
		fprintf(stderr, "browser: %p\n", browser);
		fprintf(stderr, "tab_strip_model: %p\n", browser->tab_strip_model());
		//browser->tab_strip_model()->AddObserver(this);
	}

	virtual ~BerkeliumBrowserWindow() {
	}

private:
	Browser* browser;

/*
	virtual void ActiveTabChanged(TabContents* old_contents, TabContents* new_contents, int index, bool user_gesture) {
		fprintf(stderr, "BerkeliumBrowserWindow::%s %p %p %d %s\n", __func__, (void*)old_contents, (void*)new_contents, index, user_gesture ? "true" : "false");
		if(new_contents != NULL) {
			new_contents->web_contents()->GetView()->SizeContents(gfx::Size(800, 600));
		}
	}
*/

	virtual bool IsActive() const {X;return true;}
	virtual bool IsMaximized() const {X;return false;}
	virtual bool IsMinimized() const {X;return false;}
	virtual bool IsFullscreen() const {X;return false;}
	virtual gfx::NativeWindow GetNativeWindow() {X;fprintf(stderr, "GetNativeBrowser\n");return NULL;}
	virtual gfx::Rect GetRestoredBounds() const {X;return gfx::Rect(640, 480);}
	virtual gfx::Rect GetBounds() const {X;return gfx::Rect(640, 480);}
	virtual void Show() {X;
	BrowserList::SetLastActive(browser);
	browser->OnWindowDidShow();
	//browser->GetActiveTabContents()
	}
	virtual void Hide() {X;}
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
	virtual void UpdateTitleBar() {X;}
	virtual void BookmarkBarStateChanged(BookmarkBar::AnimateChangeType) {X;}
	virtual void UpdateDevTools() {X;}
	virtual void UpdateLoadingAnimations(bool) {X;}
	virtual void SetStarredState(bool) {X;}
	virtual void ZoomChangedForActiveTab(bool) {X;}
	virtual void EnterFullscreen(const GURL&, FullscreenExitBubbleType) {X;}
	virtual void ExitFullscreen() {X;}
	virtual void UpdateFullscreenExitBubbleContent(const GURL&, FullscreenExitBubbleType) {X;}
	virtual bool ShouldHideUIForFullscreen() const {X;return true;}
	virtual bool IsFullscreenBubbleVisible() const {X;return false;}
	virtual LocationBar* GetLocationBar() const {X;return NULL;}
	virtual void SetFocusToLocationBar(bool) {X;}
	virtual void UpdateReloadStopState(bool, bool) {X;}
	virtual void UpdateToolbar(content::WebContents*, bool) {X;}
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
	virtual void ShowPageInfo(content::WebContents*, const GURL&, const content::SSLStatus&, bool) {X;}
	virtual void ShowPasswordGenerationBubble(const gfx::Rect&, const content::PasswordForm&, autofill::PasswordGenerator*) {X;}
	virtual void ShowWebsiteSettings(Profile*, content::WebContents*, const GURL&, const content::SSLStatus&, bool) {X;}
	virtual void ShowAppMenu() {X;}
	virtual bool PreHandleKeyboardEvent(const content::NativeWebKeyboardEvent&, bool*) {X;return false;}
	virtual void HandleKeyboardEvent(const content::NativeWebKeyboardEvent&) {X;}
	virtual void ShowCreateChromeAppShortcutsDialog(Profile*, const extensions::Extension*) {X;}
	virtual void Cut() {X;}
	virtual void Copy() {X;}
	virtual void Paste() {X;}
	virtual gfx::Rect GetInstantBounds() {X;return gfx::Rect(640, 480);}
	virtual bool IsInstantTabShowing() {X;return false;}
	virtual WindowOpenDisposition GetDispositionForPopupBounds(const gfx::Rect&) {X;return WindowOpenDisposition();}
	virtual FindBar* CreateFindBar() {X;return NULL;}
	virtual bool GetConstrainedWindowTopY(int*) {X;return false;}
	virtual void ShowAvatarBubble(content::WebContents*, const gfx::Rect&) {X;}
	virtual void ShowAvatarBubbleFromAvatarButton() {X;}
	virtual void DestroyBrowser() {X;}
};

BrowserWindow* BerkeliumCreateBrowserWindow(Browser* browser);

// static
BrowserWindow* BrowserWindow::CreateBrowserWindow(Browser* browser) {
	BrowserWindow* ret;

	//fprintf(stderr, "CreateBrowserWindow start\n");
	if(Berkelium::BerkeliumHost::isActive()) {
		ret = new BerkeliumBrowserWindow(browser);
	} else {
		ret = BerkeliumCreateBrowserWindow(browser);
	}
	//fprintf(stderr, "CreateBrowserWindow done\n");
	return ret;
}
