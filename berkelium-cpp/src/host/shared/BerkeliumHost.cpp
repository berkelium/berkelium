// Copyright (c) 2012 The Berkelium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "BerkeliumHost.hpp"
#include "BerkeliumHostInstance.hpp"
#include "BerkeliumHostWindow.hpp"
#include "BerkeliumHostDelegate.hpp"

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <Berkelium/API/Logger.hpp>
#include <Berkelium/API/BerkeliumFactory.hpp>
#include <Berkelium/API/Runtime.hpp>
#include <Berkelium/API/Profile.hpp>
#include <Berkelium/API/Util.hpp>
#include <Berkelium/IPC/Channel.hpp>
#include <Berkelium/IPC/LinkGroup.hpp>
#include <Berkelium/IPC/ChannelGroup.hpp>
#include <Berkelium/IPC/Message.hpp>
#include <Berkelium/Impl/Impl.hpp>
#include <Berkelium/Impl/UpdateQueue.hpp>

#include <set>
#include <map>
#include <iostream>
#include <fstream>

#ifdef BERKELIUM_NO_LIBRARY_TIMEOUT
#define USE_TIMEOUT 0
#else
#define USE_TIMEOUT 1
#endif

namespace Berkelium {

using Ipc::CommandId;
using Ipc::Channel;
using Ipc::ChannelRef;
using Ipc::ChannelGroup;
using Ipc::ChannelGroupRef;
using Ipc::LinkGroup;
using Ipc::LinkGroupRef;
using Ipc::Message;
using Ipc::MessageRef;
using impl::UpdateQueue;
using impl::UpdateQueueRef;

bool berkeliumDone = false;
BerkeliumHostInstanceRef berkeliumHostInstance;

LinkGroupRef group(LinkGroup::create());
UpdateQueueRef updates(new UpdateQueue(group));
ChannelGroupRef channels;

LoggerRef logger = Berkelium::Util::createRootLogger(NULL);
ChannelRef ipc;
ChannelRef ping;

int initialised = 0;

std::string berkeliumDebugFile;

/*
std::set<Browser*> newBrowsers;
std::set<Browser*> browsers;
std::set<Browser*> closed;

ChannelRef nextWindowChannel;
ChannelRef nextTabChannel;

*/
ChannelRef BerkeliumHost::addWindow(void* browser) {
	/*
	if(nextWindowChannel) {
		logger->debug("addWindow: requested ID");
	} else {
		logger->info("addWindow: new ID");
		nextWindowChannel = ipc->createSubChannel("window");
	}
	ChannelRef ret(nextWindowChannel);
	nextWindowChannel.reset();
	browserMap.insert(browserMap.end(), std::pair<Browser*,ChannelRef>((Browser*)browser, ret));
	return ret;
	*/
	logger->info("addWindow");
	return ChannelRef();
}

void BerkeliumHost::removeWindow(void*) {
	logger->info("removeWindow");
}

Ipc::ChannelRef BerkeliumHost::addTab(void*) {
	/*
	if(nextTabChannel) {
		logger->debug("addTab: requested ID");
	} else {
		logger->info("addTab: new ID");
		nextTabChannel = ipc->createSubChannel("tab");
	}
	ChannelRef ret(nextTabChannel);
	nextTabChannel.reset();
	return ret;
	*/
	logger->info("addTab");
	return ChannelRef();
}

void BerkeliumHost::removeTab(void*) {
	logger->info("removeTab");
}
/*

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
	//Browser* browser(chrome::OpenEmptyWindow(profile, any->host_desktop_type()));
	Browser* browser = new Browser(Browser::CreateParams(Browser::TYPE_TABBED, profile, chrome::HostDesktopType::HOST_DESKTOP_TYPE_FIRST));
	//AddBlankTabAt(browser, -1, true);
	browser->window()->Show();
	nextWindowChannel.reset();
	newBrowsers.erase(browser);
	browsers.insert(browser);
}
*/

int64_t lastPing = 0;

bool BerkeliumHost::update(int32_t timeout) {
	static bool started_send = false;

	int64_t now = Util::currentTimeMillis();

	if(!started_send) {
		// send berkelium ipc startup message
		MessageRef msg(Message::create(logger));
		started_send = true;
		msg->add_str("berkelium");
		// send ping channel id
		msg->add_32(ping->getId());
		ipc->send(msg);
		msg = ipc->recv();
		logger->info("berkelium host is up and running!");
		lastPing = now;
	}

	if(now - lastPing > 2000) {
		lastPing = now;
		MessageRef msg(Message::create(logger));
		msg->add_cmd(CommandId::ping);
		ping->send(msg);
	}

	int64_t lastRecv = group->getLastRecv();
	if(lastRecv != 0 && now - lastRecv > 4000) {
		static bool timeout = false;
		if(!timeout) {
			timeout = true;
			logger->error("berkelium library timeout!");
			if(USE_TIMEOUT) {
				exit(1);
			}
		}
	}

	/*
	for(std::set<Browser*>::iterator it(closed.begin()); it != closed.end(); it++) {
		Browser* browser(*it);
		delete browser;
	}
	closed.clear();
	*/

	updates->update(timeout);
	/*
	for(std::map<Browser*,ChannelRef>::iterator it = browserMap.begin(); it != browserMap.end(); it++) {
		process(it->second, it->first);
		//process(it->second->getReverseChannel(), it->first);
		//fprintf(stderr, "browser %s\n", it->second->getName().c_str());
	}

	process(ipc, NULL);
	*/

	if(BerkeliumHost::isDone()) {
		berkeliumHostInstance.reset();
		BerkeliumHostDelegate::exit();
		ipc.reset();
		if(!berkeliumDebugFile.empty()) {
			std::remove(berkeliumDebugFile.c_str());
		}
		return false;
	}
	BerkeliumHostDelegate::updateLater();
	return true;
}

ProfileRef BerkeliumHost::initDebug(const std::string& file) {
	std::ifstream cmdfile(file);
	std::string cmd;
	std::getline(cmdfile,  cmd);
	std::string dir = Util::getOption(cmd, "--user-data-dir=");
	std::string name = Util::getOption(cmd, "--berkelium=");
	if(name.empty() || dir.empty()) {
		fprintf(stderr, "berkelium debug error!\n");
		return ProfileRef();
	}
	berkeliumDebugFile = file;
	return init(dir, name);
}

ProfileRef BerkeliumHost::init(const std::string& dir, const std::string& name) {
	fprintf(stderr, "berkelium init %s %s!\n", dir.c_str(), name.c_str());
	/*
	for(int i = 0; i < 100; i++) {
		fprintf(stdout, "stdout %d!\n", i);
		fprintf(stderr, "stderr %d!\n", i);
		if((i + 1) % 10 == 0) {
			fprintf(stdout, "test...");
			fflush(stdout);
			Berkelium::Util::sleep(500);
		}
	}
	*/
	if(isActive()) {
		fprintf(stderr, "berkelium init double call!\n");
		return ProfileRef();
	}
	initialised = 1;
	Berkelium::impl::enableBerkeliumHostMode();
	BerkeliumHostDelegate::init();
	channels = ChannelGroup::createGroup(logger, dir, name, "host", group, false);
	if(!channels) {
		fprintf(stderr, "failed to create berkelium channel group!\n");
		return ProfileRef();
	}
	ipc = channels->getChannel(0, "berkelium-host-ipc");
	ping = channels->createChannel("ping");

	berkeliumHostInstance = BerkeliumHostInstance::createBerkeliumHostInstance(logger, ipc);

	RuntimeRef runtime(BerkeliumFactory::createRuntime());
	ProfileRef profile(runtime->forProfilePath(dir));

	if(!profile->isFound()) {
		logger->error() << "profile not found!" << std::endl;
		return ProfileRef();
	}

	if(profile->isInUse()) {
		logger->error() << "profile already in use!" << std::endl;
		return ProfileRef();
	}

	return profile;
}

void BerkeliumHost::lasyInit() {
	static bool started = false;
	if(!started) {
		started = true;
		update(0);
	}
}

bool BerkeliumHost::isActive() {
	fprintf(stderr, "berkelium isActive? %s!\n", initialised != 0 ? "true" : "false");
	return initialised != 0;
}

bool BerkeliumHost::isDone() {
	return berkeliumDone;
}

void BerkeliumHost::setDone() {
	berkeliumDone = true;
}

void BerkeliumHost::addUpdateEvent(UpdateRef update, int32_t timeout)
{
	updates->addUpdateEvent(update, timeout);
}

void BerkeliumHost::removeUpdateEvent(UpdateRef update)
{
	updates->removeUpdateEvent(update);
}

} // namespace Berkelium
