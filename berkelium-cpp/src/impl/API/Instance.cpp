// Copyright (c) 2012 The Berkelium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Berkelium/API/Profile.hpp>
#include <Berkelium/API/Instance.hpp>
#include <Berkelium/Impl/Impl.hpp>
#include <Berkelium/IPC/Ipc.hpp>
#include <Berkelium/IPC/IpcMessage.hpp>

#include <iostream>

namespace Berkelium {

Instance::Instance() {
}

Instance::~Instance() {
}

namespace impl {

class InstanceImpl : public Instance {
	HostExecutableRef executable;
	ProfileRef profile;
	IpcRef ipc;
	ProcessRef process;
	std::vector<IpcRef> windows;

public:
	InstanceImpl(HostExecutableRef executable, ProfileRef profile, IpcRef ipc, ProcessRef process) :
		executable(executable),
		profile(profile),
		ipc(ipc),
		process(process),
		windows() {
		windows.push_back(ipc);
	}

	~InstanceImpl() {
		// TODO only call close if ipc is open...
		close();
		if(profile->isInUse()) {
			std::cerr << "waiting for profile..." << std::endl;
			while(profile->isInUse()) {
				impl::sleep(100);
			}
			std::cerr << "profile closed!" << std::endl;
		}
	}

	virtual void close() {
		IpcMessageRef msg = IpcMessage::create();
		msg->add_str("exit");
		ipc->send(msg);
		//ipc->recv(msg); //ACK
	}

	virtual void update() {
		std::vector<IpcRef> copy(windows);
		for(std::vector<IpcRef>::iterator it = copy.begin(); it != copy.end(); it++) {
			IpcRef ir = *it;
			if(!ir->isEmpty()) {
				IpcMessageRef msg = IpcMessage::create();
				ir->recv(msg);
				std::string str = msg->get_str();
				if(str.compare("addWindow") == 0) {
					std::string id = msg->get_str();
					std::cerr << "new window: '" << id << "'!" << std::endl;
					IpcRef c = ipc->getChannel(id);
					windows.push_back(c);
				} else if(str.compare("OnReady") == 0) {
					msg->reset();
					msg->add_str("Navigate");
					msg->add_str("http://heise.de/");
					std::cerr << "sending navigate to heise.de!" << std::endl;
					ir->send(msg);
				} else {
					std::cerr << "recv from host: '" << str << "'!" << std::endl;
				}
			}
		}
	}

	virtual ProfileRef getProfile() {
		return ProfileRef();
	}

	virtual HostExecutableRef getExecutable() {
		return HostExecutableRef();
	}

	virtual void addLogDelegate(LogDelegateRef delegate) {
	}

	virtual void removeLogDelegate(LogDelegateRef delegate) {
	}

	virtual void addHostDelegate(HostDelegateRef delegate) {
	}

	virtual void removeHostDelegate(HostDelegateRef delegate) {
	}

	virtual void log(LogType type, const std::string& message) {
	}

	virtual int32_t getWindowCount() {
		return 0;
	}

	virtual WindowList getWindowList() {
		return WindowList();
	}

	virtual WindowRef createWindow(bool incognito) {
		return WindowRef();
	}
};

InstanceRef newInstance(HostExecutableRef executable, ProfileRef profile, IpcRef ipc, ProcessRef process) {
	return InstanceRef(new InstanceImpl(executable, profile, ipc, process));
}

} // namespace impl

} // namespace Berkelium
