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
#include "chrome/browser/profiles/profile.h"
#include "chrome/common/url_constants.h"
#include "content/public/browser/browser_thread.h"
#include "content/public/browser/render_view_host.h"
#include "base/message_loop/message_loop.h"

namespace Berkelium {

extern LoggerRef logger;

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
	BrowserList::GetInstance(chrome::HOST_DESKTOP_TYPE_NATIVE)->AddObserver(&observer);
}

void BerkeliumHostDelegate::exit()
{
	BrowserList::GetInstance(chrome::HOST_DESKTOP_TYPE_NATIVE)->RemoveObserver(&observer);
	for(std::set<Browser*>::iterator it(browsers.begin()); it != browsers.end(); it++) {
		Browser* browser(*it);
		browser->tab_strip_model()->CloseAllTabs();
		delete browser;
	}
	chrome::Exit();
}

void* BerkeliumHostDelegate::createWindow(bool incognito)
{
	if(!any) {
		fprintf(stderr, "no browser found!\n");
	}
	fprintf(stderr, "window create!\n");
	::Profile* profile;
	if(incognito) {
		profile = any->profile()->GetOffTheRecordProfile();
	} else {
		profile = any->profile()->GetOriginalProfile();
	}
	Browser* browser = new Browser(Browser::CreateParams(Browser::TYPE_TABBED, profile, chrome::HostDesktopType::HOST_DESKTOP_TYPE_FIRST));
	browser->window()->Show();
	fprintf(stderr, "window created!\n");
	return browser;
}

void BerkeliumHostDelegate::destroyWindow(void* browser)
{
	fprintf(stderr, "window destroy!\n");
	delete (Browser*)browser;
	fprintf(stderr, "window destroyed!\n");
}

void* BerkeliumHostDelegate::createTab(void* window, BerkeliumHostTabRef tab)
{
	fprintf(stderr, "tab create!\n");
	chrome::NavigateParams params((Browser*)window, GURL(chrome::kChromeUINewTabURL), content::PAGE_TRANSITION_TYPED);
	params.disposition = NEW_FOREGROUND_TAB;
	params.tabstrip_index = -1;
	chrome::Navigate(&params);
	fprintf(stderr, "tab created!\n");
	setBerkeliumHostTabRef(params.target_contents->GetRenderViewHost(), tab);
	return params.target_contents;
}

void BerkeliumHostDelegate::destroyTab(void* window, void* tab)
{
	fprintf(stderr, "tab destroy!\n");
	// INFO: addToHistory = false
	chrome::CloseWebContents((Browser*)window, (content::WebContents*)tab, false);
	fprintf(stderr, "tab destroyed!\n");
}

} // namespace Berkelium
