// Copyright (c) 2013 The Berkelium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Berkelium/API/Update.hpp>
#include <Berkelium/API/Util.hpp>
#include <Berkelium/Impl/UpdateQueue.hpp>
#include <Berkelium/IPC/LinkGroup.hpp>

namespace Berkelium {

namespace impl {

UpdateQueue::UpdateQueue(Ipc::LinkGroupRef group) :
	group(group),
	updates()
{
	TRACE_OBJECT_NEW("UpdateQueue");
}

UpdateQueue::~UpdateQueue()
{
	TRACE_OBJECT_DELETE("UpdateQueue");
}

void UpdateQueue::update(int32_t timeout)
{
	while(true) {
		int64_t now = Util::currentTimeMillis();
		UpdateRefMapIt it(updates.begin());
		for(; it != updates.end(); it++) {
			if(it->first <= now) {
				it->second->update();
				it = updates.erase(it);
				if(it == updates.end()) {
					break;
				}
			} else {
				break;
			}
		}
		if(it != updates.end() && it->first < now + timeout) {
			int32_t t = it->first - now;
			group->update(t);
			timeout -= t;
			if(timeout <= 0) {
				return;
			}
			continue;
		}
		group->update(timeout);
		break;
	}
}

void UpdateQueue::addUpdateEvent(UpdateRef update, int32_t timeout)
{
	updates.insert(UpdateRefMapPair(timeout + Util::currentTimeMillis(), update));
}

void UpdateQueue::removeUpdateEvent(UpdateRef update)
{
	for(UpdateRefMapIt it = updates.begin(); it != updates.end(); it++) {
		if(it->second == update) {
			it = updates.erase(it);
			if(it == updates.end()) {
				return;
			}
		}
	}
}

} // namespace impl

} // namespace Berkelium
