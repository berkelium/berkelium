// Copyright (c) 2013 The Berkelium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Berkelium/API/Util.hpp>
#include <Berkelium/API/Profile.hpp>
#include <Berkelium/API/BerkeliumFactory.hpp>
#include <Berkelium/IPC/Message.hpp>
#include <Berkelium/IPC/Channel.hpp>
#include <Berkelium/Impl/Logger.hpp>

#include <list>

using Berkelium::BerkeliumFactory;
using Berkelium::ProfileRef;
using Berkelium::Ipc::Channel;
using Berkelium::Ipc::ChannelRef;
using Berkelium::Ipc::Message;
using Berkelium::Ipc::MessageRef;
using Berkelium::Ipc::CommandId;

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

	MessageRef msg(ipc->getMessage());
	msg->add_str("berkelium");
	ipc->send(msg);

	Berkelium::Log::info() << "host fake started!" << std::endl;

	bool running = true;
	std::list<ChannelRef> channels;
	channels.push_front(ipc);

	while(running) {
		bool empty = true;
		for(std::list<ChannelRef>::iterator it = channels.begin(); it != channels.end(); it++) {
			if(!running) {
				break;
			}
			ChannelRef ipc = *it;

			if(ipc->isEmpty()) {
				continue;
			}
			empty = false;

			ipc->recv(msg);
			bool sendAck = false;

			switch(CommandId cmd = msg->get_cmd()) {
				default: {
					Berkelium::Log::error() << "received unknown command '" << cmd << "'" << std::endl;
					sendAck = true;
					break;
				}

				case CommandId::exitHost: {
					running = false;
					sendAck = true;
					break;
				}

				case CommandId::createTab: {
					ChannelRef tab(ipc->createSubChannel());
					channels.push_back(tab);
					msg->reset();
					msg->add_cmd(CommandId::newTab);
					msg->add_str(tab->getName());
					ipc->send(msg);
					Berkelium::Log::info() << "created new tab with id " << tab->getName() << "!" << std::endl;
					break;
				}

				case CommandId::createWindow: {
					bool incognito = msg->get_8() == 1;
					ChannelRef win(ipc->createSubChannel());
					channels.push_back(win);
					msg->reset();
					msg->add_cmd(CommandId::newWindow);
					msg->add_str(win->getName());
					ipc->send(msg);
					Berkelium::Log::info() << "created new " << (incognito ? "incognito" : "default") << " window with id "
							<< win->getName() << "!" << std::endl;
					break;
				}
			}

			if(sendAck) {
				msg->reset();
				ipc->send(msg); // ACK
			}
		}
		if(empty) {
			Berkelium::Util::sleep(33);
		}
	}

	Berkelium::Log::info() << "host fake done!" << std::endl;

	return 0;
}
