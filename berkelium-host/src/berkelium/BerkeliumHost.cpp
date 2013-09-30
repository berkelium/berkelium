// Copyright (c) 2012 The Berkelium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "BerkeliumHost.hpp"

#include "chrome/browser/ui/browser.h"
#include "chrome/browser/ui/browser_list.h"
#include "chrome/browser/ui/browser_list_observer.h"
#include "chrome/browser/ui/browser_commands.h"
#include "chrome/browser/profiles/profile.h"
#include "content/public/browser/browser_thread.h"
#include "base/message_loop.h"

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <Berkelium/API/Logger.hpp>
#include <Berkelium/API/Util.hpp>
#include <Berkelium/IPC/Channel.hpp>
#include <Berkelium/IPC/ChannelGroup.hpp>
#include <Berkelium/IPC/Message.hpp>
#include <Berkelium/Impl/Impl.hpp>

#include <set>
#include <map>
#include <iostream>

namespace Berkelium {

using Ipc::CommandId;
using Ipc::Channel;
using Ipc::ChannelRef;
using Ipc::ChannelGroup;
using Ipc::ChannelGroupRef;
using Ipc::Message;
using Ipc::MessageRef;

ChannelGroupRef group(ChannelGroup::create());

LoggerRef logger = Berkelium::Util::createRootLogger(NULL);
ChannelRef ipc;
MessageRef msg;

std::set<Browser*> newBrowsers;
std::set<Browser*> browsers;
std::set<Browser*> closed;
Browser* any;
std::map<Browser*,ChannelRef> browserMap;

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

		newBrowsers.insert(browser);
	}

	virtual void OnBrowserRemoved(::Browser* browser) {
		logger->info("OnBrowserRemoved");
		newBrowsers.erase(browser);
		browsers.erase(browser);

		std::map<Browser*,ChannelRef>::iterator it(browserMap.find(browser));
		if(it != browserMap.end()) {
			browserMap.erase(it);
		}

		if(any == browser) {
			if(browsers.empty()) {
				if(newBrowsers.empty()) {
					any = NULL;
				} else {
					any = *(newBrowsers.begin());
				}
			} else {
				any = *(browsers.begin());
			}
		}
	}
};

BerkeliumBrowserListObserver observer;

ChannelRef nextWindowChannel;

ChannelRef BerkeliumHost::addWindow(void* browser) {
	if(nextWindowChannel) {
		logger->debug("addWindow: requested ID");
	} else {
		logger->info("addWindow: new ID");
		nextWindowChannel = ipc->createSubChannel();
	}
	ChannelRef ret(nextWindowChannel);
	nextWindowChannel.reset();
	browserMap.insert(browserMap.end(), std::pair<Browser*,ChannelRef>((Browser*)browser, ret));
	return ret;
}

void BerkeliumHost::removeWindow(void*) {
	logger->info("removeWindow");
}

Ipc::ChannelRef BerkeliumHost::addTab(void*) {
	logger->info("addTab");
	return ipc->createSubChannel();
}

void BerkeliumHost::removeTab(void*) {
	logger->info("removeTab");
}


void CreateWindow(Ipc::ChannelRef win, bool incognito) {
	if(!any) {
		logger->error("no window found!");
		return;
	}
	::Profile* profile;
	if(incognito) {
		profile = any->profile()->GetOffTheRecordProfile();
	} else {
		profile = any->profile()->GetOriginalProfile();
	}
	nextWindowChannel = win;
	Browser* browser(chrome::OpenEmptyWindow(profile, any->host_desktop_type()));
	nextWindowChannel.reset();
	newBrowsers.erase(browser);
	browsers.insert(browser);
}

void process(ChannelRef ipc, Browser* browser);

void update() {
	static bool started_send = false;

	if(!started_send) {
		// send berkelium ipc startup message
		logger->info("berkelium host is up and running!");
		started_send = true;
		msg->add_str("berkelium");
		ipc->send(msg);
	}

	for(std::set<Browser*>::iterator it(closed.begin()); it != closed.end(); it++) {
		Browser* browser(*it);
		delete browser;
	}
	closed.clear();

	for(std::map<Browser*,ChannelRef>::iterator it = browserMap.begin(); it != browserMap.end(); it++) {
		process(it->second, it->first);
		//process(it->second->getReverseChannel(), it->first);
		//fprintf(stderr, "browser %s\n", it->second->getName().c_str());
	}

	process(ipc, NULL);

	content::BrowserThread::PostDelayedTask(content::BrowserThread::UI, FROM_HERE, base::Bind(&update), base::TimeDelta::FromMilliseconds(10));
}

void process(ChannelRef ipc, Browser* browser) {
	if(!ipc->isEmpty()) {
		fprintf(stderr, "ipc\n");
		ipc->recv(msg);
		bool sendAck = false;

		switch(CommandId cmd = msg->get_cmd()) {
			default: {
				logger->error() << "received unknown command '" << cmd << "'" << std::endl;
				sendAck = true;
				break;
			}

			case CommandId::exitHost: {
				logger->info() << "berkelium host shutdown started!" << std::endl;
				chrome::Exit();
				/*
				for(std::set<BerkeliumChromiumTabRef>::iterator it = allTabs.begin(); it != allTabs.end(); it++) {
					(*it)->CloseTab();142
				}
				for(std::set<BerkeliumChromiumWindowRef>::iterator it = allWindows.begin(); it != allWindows.end(); it++) {
					(*it)->CloseWindow();
				}
				*/
				//running = false;
				sendAck = true;
				break;
			}

			case CommandId::createTab: {
				ChannelRef tab(ipc->createSubChannel());
				ChannelRef tab2(tab->getReverseChannel());
				//channels.push_back(tab);
				msg->reset();
				msg->add_str(tab->getName());
				ipc->send(msg);
				logger->info() << "created new tab with id " << tab->getName() << "!" << std::endl;

				// wait 2s and send onReady
				//MessageRef m = Message::create(logger);
				//m->add_cmd(CommandId::onReady);
				//todo.push(Entry(logger, 2000, tab2, m));
				//msg->reset();
				break;
			}

			case CommandId::createWindow: {
				bool incognito = msg->get_8() == 1;
				ChannelRef win(ipc->createSubChannel());
				CreateWindow(win, incognito);

				msg->reset();
				msg->add_str(win->getName());
				ipc->send(msg);

				logger->info() << "created new " << (incognito ? "incognito" : "default") << " window with id "
						<< win->getName() << "!" << std::endl;
				break;
			}

			case CommandId::navigate: {
				std::string url(msg->get_str());
				logger->info() << "navigate to '" << url << "'" << std::endl;
				break;
			}
		}
		if(sendAck) {
			msg->reset();
			ipc->send(msg); // ACK
		}
	}
}

static int initialised = 0;

bool BerkeliumHost::init(const std::string& dir, const std::string& name) {
	if(isActive()) {
		fprintf(stderr, "berkelium init double call!\n");
		return false;
	}
	initialised = 1;
	Berkelium::impl::enableBerkeliumHostMode();
	BrowserList::GetInstance(chrome::HOST_DESKTOP_TYPE_NATIVE)->AddObserver(&observer);
	ipc = Channel::getChannel(group, logger, dir, name, false);
	msg = ipc->getMessage();
	return true;
}

void BerkeliumHost::lasyInit() {
	static bool started = false;
	if(!started) {
		started = true;
		update();
	}
}

void BerkeliumHost::destory() {
	BrowserList::GetInstance(chrome::HOST_DESKTOP_TYPE_NATIVE)->RemoveObserver(&observer);
	ipc.reset();
	fprintf(stderr, "berkelium closed!\n");
}

bool BerkeliumHost::isActive() {
	return initialised != 0;
}

} // namespace Berkelium
