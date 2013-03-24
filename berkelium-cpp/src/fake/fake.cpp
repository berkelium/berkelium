// Copyright (c) 2013 The Berkelium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Berkelium/API/Util.hpp>
#include <Berkelium/API/Profile.hpp>
#include <Berkelium/API/BerkeliumFactory.hpp>
#include <Berkelium/IPC/Message.hpp>
#include <Berkelium/IPC/Channel.hpp>
#include <Berkelium/Impl/Logger.hpp>

using Berkelium::BerkeliumFactory;
using Berkelium::ProfileRef;
using Berkelium::Ipc::Channel;
using Berkelium::Ipc::ChannelRef;
using Berkelium::Ipc::Message;
using Berkelium::Ipc::MessageRef;

using Berkelium::Util::getOption;

int main(int argc, char* argv[])
{
	Berkelium::Log::setPrefix("Host");

	std::string dir = getOption(argc, argv, "--user-data-dir=");
	std::string id = getOption(argc, argv, "--berkelium=");

	if(dir.empty() || id.empty()) {
		Berkelium::Log::error() << "profile or id not given!" << std::endl;
		return 1;
	}

	ProfileRef profile = BerkeliumFactory::forProfilePath(dir);
	if(!profile->isFound()) {
		Berkelium::Log::error() << "profile not found!" << std::endl;
		return 1;
	}
	if(profile->isInUse()) {
		Berkelium::Log::error() << "profile already in use!" << std::endl;
		return 1;
	}
	profile->setLocked(true);
	if(!profile->isLocked()) {
		Berkelium::Log::error() << "profile can not be locked!" << std::endl;
		return 1;
	}

	Berkelium::Log::debug() << "starting host fake!" << std::endl;
	ChannelRef ipc = Channel::getChannel(dir, id, false);

	MessageRef recv = Message::create();
	MessageRef send = Message::create();
	send->add_str("berkelium");
	ipc->send(send);

	Berkelium::Log::info() << "host fake started!" << std::endl;

	bool running = true;
	while(running) {
		if(ipc->isEmpty()) {
			Berkelium::Util::sleep(250);
			continue;
		}
		ipc->recv(recv);
		std::string cmd = recv->get_str();
		Berkelium::Log::debug() << "recv: '" << cmd << "'" << std::endl;

		if(cmd.compare("exit") == 0) {
			running = false;
		} else if(cmd.compare("openWindow") == 0) {
			bool incognito = recv->get_8() == 1;
			ChannelRef win(ipc->createSubChannel());
			send->reset();
			send->add_str("addWindow");
			send->add_str(win->getName());
			ipc->send(send);
			Berkelium::Log::info() << "created new " << (incognito ? "incognito" : "default") << " window with id "
					<< win->getName() << "!" << std::endl;
		}

		send->reset();
		ipc->send(send); // ACK
	}

	Berkelium::Log::info() << "host fake done!" << std::endl;

	return 0;
}
