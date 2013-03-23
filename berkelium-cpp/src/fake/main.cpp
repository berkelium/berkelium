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

	std::cerr << "starting host fake!" << std::endl;
	ChannelRef ipc = Channel::getChannel(dir, id, false);

	MessageRef recv = Message::create();
	MessageRef send = Message::create();
	send->add_str("berkelium");
	ipc->send(send);

	std::cerr << "host fake started!" << std::endl;

	ChannelRef win(ipc->createSubChannel());
	send->reset();
	send->add_str("addWindow");
	send->add_str(win->getName());
	ipc->send(send);

	bool running = true;
	while(running) {
		if(ipc->isEmpty()) {
			Berkelium::Util::sleep(250);
			continue;
		}
		ipc->recv(recv);
		std::string cmd = recv->get_str();
		std::cout << "recv: '" << cmd << "'" << std::endl;

		if(cmd.compare("exit") == 0) {
			running = false;
		}

		send->reset();
		ipc->send(send); // ACK
	}

	std::cerr << "host fake done!" << std::endl;

	return 0;
}
