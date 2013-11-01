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
#include <Berkelium/API/HostDelegate.hpp>
#include <Berkelium/API/Update.hpp>
#include <Berkelium/IPC/Channel.hpp>
#include <Berkelium/IPC/Message.hpp>
#include <Berkelium/IPC/LinkGroup.hpp>
#include <Berkelium/IPC/Link.hpp>
#include <Berkelium/IPC/ChannelGroup.hpp>
#include <Berkelium/Impl/Impl.hpp>
#include <Berkelium/Impl/Manager.hpp>

#include <set>

namespace Berkelium {

Instance::Instance() {
	TRACE_OBJECT_NEW("Instance");
}

Instance::~Instance() {
	TRACE_OBJECT_DELETE("Instance");
}

namespace impl {

template<typename T>
struct set {
	typedef std::set<T, std::owner_less<T>> type;
};

typedef set<WindowWRef>::type WindowWRefSet;
typedef set<HostDelegateWRef>::type HostDelegateWRefSet;

class InstancePinger : public Update
{
private:
	InstanceWRef instance;

public:
	InstancePinger(InstanceWRef instance);

	virtual ~InstancePinger();

	void update();
};

typedef std::shared_ptr<InstancePinger> InstancePingerRef;

class InstanceImpl : public Instance, public Berkelium::Ipc::ChannelCallback {
BERKELIUM_IMPL_CLASS(Instance)

private:
	InstanceWRef self;
	Ipc::ChannelGroupRef group;
	HostExecutableRef executable;
	ProfileRef profile;
	Ipc::ChannelRef send;
	Ipc::ChannelRef recv;
	Ipc::ChannelRef ping;
	Ipc::MessageRef pingMsg;
	InstancePingerRef pinger;
	Berkelium::Ipc::ChannelCallbackRef cb;
	ProcessRef process;
	WindowWRefSet windows;
	HostDelegateWRefSet hosts;

public:
	InstanceImpl(HostExecutableRef executable, ProfileRef profile, Ipc::ChannelRef ipc, ProcessRef process, Ipc::ChannelRef ping) :
		BERKELIUM_IMPL_CTOR3(Instance, ipc->getAlias(), executable),
		self(),
		group(ipc->getGroup()),
		executable(executable),
		profile(profile),
		send(ipc),
		recv(ipc->getReverseChannel()),
		ping(ping),
		pingMsg(Ipc::Message::create(logger)),
		pinger(),
		cb(),
		process(process),
		windows(),
		hosts() {
		TRACE_OBJECT_NEW("InstanceImpl");
	}

	~InstanceImpl() {
		if(pinger) {
			runtime->removeUpdateEvent(pinger);
		}
		TRACE_OBJECT_DELETE("InstanceImpl");
		getManager()->unregisterInstance();
		// TODO only call close if ipc is open...
		close();
		if(profile->isInUse()) {
			logger->debug() << "closing instance: waiting for profile..." << std::endl;
			int timeout = 20000;
			while(profile->isInUse() && timeout > 0) {
				runtime->update(100);
				timeout-=100;
			}
			if(timeout < 0) {
				logger->error() << "profile not closed!" << std::endl;
			} else {
				logger->debug() << "profile closed!" << std::endl;
			}
		}
	}

	void setSelf(InstanceRef instance) {
		self = instance;
	}

	void sendPing() {
		if(!process->isRunning()) {
			if(process->isCrashed()) {
				onCrashed();
			} else {
				onClosed();
			}
			return;
		}

		pingMsg->reset();
		pingMsg->add_cmd(Ipc::CommandId::ping);
		ping->send(pingMsg);

		pinger = InstancePingerRef(new InstancePinger(self));
		runtime->addUpdateEvent(pinger, 250);
	}

	virtual void close() {
		Ipc::MessageRef message(Ipc::Message::create(logger));
		message->add_cmd(Ipc::CommandId::exitHost);
		send->send(message);
	}

	virtual void onChannelDataReady(Ipc::ChannelRef channel, Ipc::MessageRef message) {
		switch(Ipc::CommandId cmd = message->get_cmd()) {
			default: {
				logger->error() << "Instance: received unknown command '" << cmd << "'" << std::endl;
				break;
			}
			case Ipc::CommandId::ping: {
				onPing();
				break;
			}
		}
	}

	void onPing() {
		for(HostDelegateWRefSet::iterator it(hosts.begin()); it != hosts.end(); it++) {
			HostDelegateRef ref = it->lock();
			if(ref) {
				ref->onPing(self.lock());
			}
		}
	}

	void onCrashed() {
		for(HostDelegateWRefSet::iterator it(hosts.begin()); it != hosts.end(); it++) {
			HostDelegateRef ref = it->lock();
			if(ref) {
				ref->onCrashed(self.lock());
			}
		}
	}

	void onClosed() {
		for(HostDelegateWRefSet::iterator it(hosts.begin()); it != hosts.end(); it++) {
			HostDelegateRef ref = it->lock();
			if(ref) {
				ref->onClosed(self.lock());
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

		Ipc::MessageRef message(Ipc::Message::create(logger));
		message->add_cmd(Ipc::CommandId::createWindow);
		message->add_8(incognito);
		send->send(message);
		message = send->recv();

		int32_t id = message->get_32();
		logger->debug() << "created window '" << id << "'!" << std::endl;
		Ipc::ChannelRef channel = group->getChannel(id, "window");

		WindowRef ret(newWindow(self.lock(), channel, incognito));
		windows.insert(ret);

		return ret;
	}

	void debugHost(int8_t cmd) {
		Ipc::MessageRef message(Ipc::Message::create(logger));
		message->add_cmd(Ipc::CommandId::debug);
		message->add_8(cmd);
		send->send(message);
	}

	static InstanceRef newInstance(HostExecutableRef executable, ProfileRef profile, Ipc::ChannelRef ipc, ProcessRef process, Ipc::ChannelRef ping) {
		InstanceImpl* impl = new InstanceImpl(executable, profile, ipc, process, ping);
		InstanceRef ret(impl);
		impl->setSelf(ret);
		impl->cb.reset(new Berkelium::Ipc::ChannelCallbackDelegate<Instance, InstanceImpl>(ret));
		impl->send->registerCallback(impl->cb);
		impl->recv->registerCallback(impl->cb);
		impl->ping->registerCallback(impl->cb);
		impl->getManager()->registerInstance(ret);
		//impl->createWindow(false);
		impl->sendPing();
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

InstanceRef newInstance(HostExecutableRef executable, ProfileRef profile, Ipc::ChannelRef ipc, ProcessRef process, Ipc::ChannelRef ping) {
	return InstanceImpl::newInstance(executable, profile, ipc, process, ping);
}

InstancePinger::InstancePinger(InstanceWRef instance) :
	Update(),
	instance(instance)
{
	TRACE_OBJECT_NEW("InstancePinger");
}

InstancePinger::~InstancePinger()
{
	TRACE_OBJECT_DELETE("InstancePinger");
}

void InstancePinger::update()
{
	InstanceRef locked(instance.lock());
	if(locked) {
		((InstanceImpl*)locked.get())->sendPing();
	}
}

void testHostCrash(InstanceRef instance)
{
	if(instance) {
		InstanceImpl* impl = (InstanceImpl*)instance.get();
		impl->debugHost(0);
	}
}

void testHostHang(InstanceRef instance)
{
	if(instance) {
		InstanceImpl* impl = (InstanceImpl*)instance.get();
		impl->debugHost(1);
	}
}

} // namespace impl

} // namespace Berkelium
