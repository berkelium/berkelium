// Copyright (c) 2012 The Berkelium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Berkelium/API/Profile.hpp>
#include <Berkelium/API/HostExecutable.hpp>
#include <Berkelium/API/Instance.hpp>
#include <Berkelium/API/Util.hpp>
#include <Berkelium/API/Runtime.hpp>
#include <Berkelium/API/Logger.hpp>
#include <Berkelium/API/Window.hpp>
#include <Berkelium/IPC/Channel.hpp>
#include <Berkelium/IPC/Message.hpp>
#include <Berkelium/Impl/Process.hpp>
#include <Berkelium/Impl/Impl.hpp>
#include <Berkelium/Impl/Manager.hpp>

#include <set>

namespace Berkelium {

Instance::Instance() {
}

Instance::~Instance() {
}

namespace impl {

template<typename T>
struct set {
	typedef std::set<T, std::owner_less<T>> type;
};

typedef set<WindowWRef>::type WindowWRefSet;
typedef set<HostDelegateWRef>::type HostDelegateWRefSet;

class InstanceImpl : public Instance {
BERKELIUM_IMPL_CLASS(Instance)

private:
	InstanceWRef self;
	HostExecutableRef executable;
	ProfileRef profile;
	Ipc::ChannelRef send;
	Ipc::ChannelRef recv;
	Ipc::MessageRef message;
	ProcessRef process;
	WindowWRefSet windows;
	HostDelegateWRefSet hosts;

public:
	InstanceImpl(HostExecutableRef executable, ProfileRef profile, Ipc::ChannelRef ipc, ProcessRef process) :
		BERKELIUM_IMPL_CTOR3(Instance, ipc->getName(), executable),
		self(),
		executable(executable),
		profile(profile),
		send(ipc),
		recv(ipc->getReverseChannel()),
		message(ipc->getMessage()),
		process(process),
		windows(),
		hosts() {
	}

	~InstanceImpl() {
		getManager()->unregisterInstance();
		// TODO only call close if ipc is open...
		close();
		if(profile->isInUse()) {
			logger->debug() << "closing instance: waiting for profile..." << std::endl;
			while(profile->isInUse()) {
				Util::sleep(100);
			}
			logger->debug() << "profile closed!" << std::endl;
		}
	}

	void setSelf(InstanceRef instance) {
		self = instance;
	}

	virtual void close() {
		message->reset();
		message->add_cmd(Ipc::CommandId::exitHost);
		send->send(message);
		//ipc->recv(message); //ACK
	}

	virtual void internalUpdate() {
		if(!recv->isEmpty()) {
			recv->recv(message);
			if(message->length() == 0) {
				// only an ack..
			} else {
				switch(Ipc::CommandId cmd = message->get_cmd()) {
					default: {
						logger->error() << "Instance: received unknown command '" << cmd << "'" << std::endl;
						break;
					}
				}
			}
		}
		for(WindowWRefSet::iterator it = windows.begin(); it != windows.end(); it++) {
			WindowRef win(it->lock());
			if(win) {
				win->internalUpdate();
			} else {
				it = windows.erase(it);
			}
		}
	}

	virtual ProfileRef getProfile() {
		return profile;
	}

	virtual HostExecutableRef getExecutable() {
		return executable;
	}

	virtual void addHostDelegate(HostDelegateRef delegate) {
		hosts.insert(delegate);
	}

	virtual void removeHostDelegate(HostDelegateRef delegate) {
		for(HostDelegateWRefSet::iterator it(hosts.begin()); it != hosts.end(); it++) {
			HostDelegateRef ref = it->lock();
			if(!ref || ref.get() == delegate.get()) {
				it = hosts.erase(it);
			}
		}
	}

	virtual int32_t getWindowCount() {
		return windows.size();
	}

	virtual WindowListRef getWindowList() {
		WindowListRef ret(new WindowList());
		for(WindowWRefSet::iterator it(windows.begin()); it != windows.end(); it++) {
			WindowRef ref = it->lock();
			if(ref) {
				ret->push_back(ref);
			} else {
				it = windows.erase(it);
			}
		}
		return ret;
	}

	virtual WindowRef createWindow(bool incognito) {
		logger->debug() << "create Window start" << std::endl;

		message->reset();
		message->add_cmd(Ipc::CommandId::createWindow);
		message->add_8(incognito);
		send->send(message);
		send->recv(message);

		std::string id = message->get_str();
		logger->debug() << "created window '" << id << "'!" << std::endl;
		Ipc::ChannelRef channel = send->getSubChannel(id);

		WindowRef ret(newWindow(self.lock(), channel, incognito));
		windows.insert(ret);

		return ret;
	}
};

ManagerRef getManager(Instance* instance)
{
	if(!instance) {
		bk_error("getManager(Instance* = null)");
		return ManagerRef();
	}
	InstanceImpl* impl = (InstanceImpl*)instance;
	return impl->getManager();
}

InstanceRef newInstance(HostExecutableRef executable, ProfileRef profile, Ipc::ChannelRef ipc, ProcessRef process) {
	InstanceImpl* impl = new InstanceImpl(executable, profile, ipc, process);
	InstanceRef ret(impl);
	impl->setSelf(ret);
	impl->getManager()->registerInstance(ret);
	//impl->createWindow(false);
	return ret;
}

} // namespace impl

} // namespace Berkelium
