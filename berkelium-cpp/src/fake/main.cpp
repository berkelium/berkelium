// Copyright (c) 2013 The Berkelium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Berkelium/API/Util.hpp>
#include <Berkelium/API/Profile.hpp>
#include <Berkelium/API/BerkeliumFactory.hpp>
#include <Berkelium/IPC/Message.hpp>
#include <Berkelium/IPC/Channel.hpp>

#include <iostream>

using Berkelium::BerkeliumFactory;
using Berkelium::ProfileRef;
using Berkelium::Ipc::Channel;
using Berkelium::Ipc::ChannelRef;
using Berkelium::Ipc::Message;
using Berkelium::Ipc::MessageRef;

std::string getOption(int argc, char* argv[], std::string option)
{
	for(int i = 0; i < argc; i++) {
		std::string tmp(argv[i]);
		if(tmp.compare(0, option.length(), option) == 0) {
			return tmp.substr(option.length());
		}
	}
	return "";
}

int main(int argc, char* argv[])
{
	std::string dir = getOption(argc, argv, "--user-data-dir=");
	std::string id = getOption(argc, argv, "--berkelium=");

	if(dir.empty() || id.empty()) {
		std::cerr << "profile or id not given!" << std::endl;
		return 1;
	}

	ProfileRef profile = BerkeliumFactory::forProfilePath(dir);
	if(!profile->isFound()) {
		std::cerr << "profile not found!" << std::endl;
		return 1;
	}
	if(profile->isInUse()) {
		std::cerr << "profile already in use!" << std::endl;
		return 1;
	}
	profile->setLocked(true);
	if(!profile->isLocked()) {
		std::cerr << "profile can not be locked!" << std::endl;
		return 1;
	}

	ChannelRef ipc = Channel::getChannel(dir, id, false);

	MessageRef msg = Message::create();
	msg->add_str("berkelium");
	ipc->send(msg);

	Berkelium::Util::sleep(5000);

	return 0;
}
