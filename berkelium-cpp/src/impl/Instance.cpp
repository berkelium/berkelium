// Copyright (c) 2012 The Berkelium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "berkelium/Profile.hpp"
#include "berkelium/Instance.hpp"
#include "berkelium/Impl.hpp"
#include "berkelium/Ipc.hpp"
#include "berkelium/IpcMessage.hpp"

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

public:
	InstanceImpl(HostExecutableRef executable, ProfileRef profile, IpcRef ipc, ProcessRef process) :
		executable(executable),
		profile(profile),
		ipc(ipc),
		process(process) {
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
		ipc->recv(msg); //ACK
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
