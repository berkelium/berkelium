// Copyright (c) 2013 The Berkelium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BERKELIUM_IMPL_BERKELIUM_CALLBACK_HPP_
#define BERKELIUM_IMPL_BERKELIUM_CALLBACK_HPP_
#pragma once

#ifndef BERKELIUM_CPP_IMPL
# error "This file is intended for internal use only!"
#endif

#include <Berkelium/IPC/Ipc.hpp>

namespace Berkelium {

namespace impl {

class InternalUpdate {
public:
	virtual ~InternalUpdate() {
	}

	virtual void internalUpdate() = 0;
};

template<class T, class I>
class BerkeliumCallback : public Berkelium::Ipc::ChannelCallback {
private:
	std::weak_ptr<T> wref;

public:
	BerkeliumCallback(std::weak_ptr<T> wref) :
		Berkelium::Ipc::ChannelCallback(),
		wref(wref) {
	}

	virtual ~BerkeliumCallback() {
	}

	virtual void onDataReady(Berkelium::Ipc::ChannelRef channel) {
		std::shared_ptr<T> ref(wref.lock());
		if(ref) {
			I* impl = (I*)ref.get();
			impl->internalUpdate();
		}
	}
};

} // namespace impl

} // namespace Berkelium

#endif // BERKELIUM_IMPL_BERKELIUM_CALLBACK_HPP_
