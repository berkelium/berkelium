// Copyright (c) 2013 The Berkelium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Berkelium/API/Logger.hpp>

#include "BerkeliumHost.hpp"
#include "BerkeliumHostDelegate.hpp"
#include "MemoryRenderViewHostFactory.hpp"

#include "chrome/browser/ui/browser.h"
#include "chrome/browser/ui/browser_list.h"
#include "chrome/browser/ui/browser_list_observer.h"
#include "chrome/browser/ui/browser_commands.h"
#include "chrome/browser/ui/browser_window.h"
#include "chrome/browser/ui/browser_tabstrip.h"
#include "chrome/browser/ui/tabs/tab_strip_model.h"
#include "chrome/browser/profiles/profile_manager.h"
#include "chrome/browser/profiles/profile.h"
#include "chrome/common/url_constants.h"
#include "content/public/browser/browser_thread.h"
#include "content/public/browser/render_view_host.h"
#include "base/message_loop/message_loop.h"

BrowserWindow* BerkeliumCreateBrowserWindow(Browser*);
//content::RenderWidgetHostView* BerkeliumCreateViewForWidget(content::RenderWidgetHost*);
content::WebContentsViewPort* BerkeliumCreateWebContentsView(content::WebContentsImpl*, content::WebContentsViewDelegate*, content::RenderViewHostDelegateView**);

extern BrowserWindow* (*BerkeliumHook_CreateBrowserWindow)(Browser*);
namespace content {
//extern content::RenderWidgetHostView* (*BerkeliumHook_CreateViewForWidget)(content::RenderWidgetHost*);
extern WebContentsViewPort* (*BerkeliumHook_CreateWebContentsView)(WebContentsImpl*, WebContentsViewDelegate*, RenderViewHostDelegateView**);
}

namespace Berkelium {

extern LoggerRef logger;

/*
::Browser* any;
std::set<Browser*> browsers;

class BerkeliumBrowserListObserver : public chrome::BrowserListObserver {
public:
	BerkeliumBrowserListObserver() :
		chrome::BrowserListObserver() {
	}

	virtual ~BerkeliumBrowserListObserver() {
	}

	virtual void OnBrowserAdded(::Browser* browser) {
		bool incognito(browser->profile()->IsOffTheRecord());
		logger->info() << "OnBrowserAdded: incognito: " << incognito << std::endl;

		if(!any) {
			any = browser;
		}

		browsers.insert(browser);
	}

	virtual void OnBrowserRemoved(::Browser* browser) {
		logger->info("OnBrowserRemoved");

		browsers.erase(browser);

		if(any == browser) {
			if(browsers.empty()) {
				any = NULL;
			} else {
				any = *(browsers.begin());
			}
		}
	}
};

BerkeliumBrowserListObserver observer;
*/

void invoke_update()
{
	BerkeliumHost::update(10);
}

void BerkeliumHostDelegate::updateLater()
{
	content::BrowserThread::PostDelayedTask(content::BrowserThread::UI, FROM_HERE, base::Bind(&invoke_update), base::TimeDelta::FromMilliseconds(10));
}

void BerkeliumHostDelegate::init()
{
	/*
	BrowserList::GetInstance(chrome::HOST_DESKTOP_TYPE_NATIVE)->AddObserver(&observer);
	*/
	BerkeliumHook_CreateBrowserWindow = BerkeliumCreateBrowserWindow;
	//content::BerkeliumHook_CreateViewForWidget = BerkeliumCreateViewForWidget;
	content::BerkeliumHook_CreateWebContentsView = BerkeliumCreateWebContentsView;
}

void BerkeliumHostDelegate::exit()
{
	/*
	BrowserList::GetInstance(chrome::HOST_DESKTOP_TYPE_NATIVE)->RemoveObserver(&observer);
	for(std::set<Browser*>::iterator it(browsers.begin()); it != browsers.end(); it++) {
		Browser* browser(*it);
		browser->tab_strip_model()->CloseAllTabs();
		delete browser;
	}
	*/
	chrome::Exit();
}

void* BerkeliumHostDelegate::createWindow(bool incognito)
{
	/*
	static bool first_closed = false;
	if(!any) {
		fprintf(stderr, "no browser found!\n");
	}
	*/
	fprintf(stderr, "window create step 1!\n");
	static ::Profile* profile(ProfileManager::GetLastUsedProfile());
	fprintf(stderr, "window create step 2!\n");
	/*
	if(!first_closed && any->profile()->IsOffTheRecord() == incognito) {
		profile = any->profile();
		fprintf(stderr, "reusing first window!\n");
		first_closed = true;
		any->tab_strip_model()->CloseAllTabs();
		return any;
	} else */
	if(incognito) {
		profile = profile->GetOffTheRecordProfile();
	} else {
		profile = profile->GetOriginalProfile();
	}
	fprintf(stderr, "window create step 3!\n");
	Browser::CreateParams params(Browser::TYPE_TABBED, profile, chrome::HOST_DESKTOP_TYPE_FIRST);
	params.initial_bounds = gfx::Rect(10, 10, 1024, 768);
	params.initial_show_state = ui::SHOW_STATE_NORMAL;
	Browser* browser = new Browser(params);
	gfx::Rect rect(browser->window()->GetBounds());
	fprintf(stderr, "window rect: %d %d %d %d!\n", rect.x(), rect.y(), rect.width(), rect.height());
	//browser->window()->SetBounds(gfx::Rect(10, 10, 1024, 768));
	browser->window()->Show();
	fprintf(stderr, "window created!\n");
	/*
	if(!first_closed) {
		first_closed = true;
		fprintf(stderr, "destory first window start!\n");
		destroyWindow(any);
		fprintf(stderr, "destory first window end!\n");
	}
	*/
	return browser;
}

void BerkeliumHostDelegate::destroyWindow(void* browser)
{
	Browser* b((Browser*)browser);
	fprintf(stderr, "window destroy all tabs!\n");
	b->tab_strip_model()->CloseAllTabs();
	fprintf(stderr, "window destroy!\n");
	delete b;
	fprintf(stderr, "window destroyed!\n");
}

void* BerkeliumHostDelegate::createTab(void* window, BerkeliumHostTabRef tab)
{
	Browser* browser((Browser*)window);
	fprintf(stderr, "=============================\n");
	fprintf(stderr, "tab create!\n");
	content::Referrer referrer;
	GURL url("http://www.google.com"/*chrome::kChromeUINewTabURL*/);
	content::OpenURLParams params(url, referrer, NEW_FOREGROUND_TAB, content::PAGE_TRANSITION_TYPED, false);
	content::WebContents* ret(browser->OpenURL(params));
	setBerkeliumHostTabRef(ret->GetRenderViewHost(), tab);
	fprintf(stderr, "tab created!\n");
	return ret;
}

void BerkeliumHostDelegate::destroyTab(void* window, void* tab)
{
	fprintf(stderr, "tab destroy!\n");
	// INFO: addToHistory = false
	chrome::CloseWebContents((Browser*)window, (content::WebContents*)tab, false);
	fprintf(stderr, "tab destroyed!\n");
}

} // namespace Berkelium
