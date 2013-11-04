// Copyright (c) 2013 The Berkelium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BERKELIUM_HOST_DELEGATE_HPP_
#define BERKELIUM_HOST_DELEGATE_HPP_
#pragma once

#include <memory>

namespace Berkelium {

class BerkeliumHostTab;
typedef std::shared_ptr<BerkeliumHostTab> BerkeliumHostTabRef;
typedef std::weak_ptr<BerkeliumHostTab> BerkeliumHostTabWRef;

class BerkeliumHostDelegate {
public:
	static void updateLater();

	static void exit();

	static void init();

	static void* createWindow(bool incognito);

	static void destroyWindow(void* window);

	static void* createTab(void* window, BerkeliumHostTabRef tab);

	static void destroyTab(void* window, void* tab);
};

} // namespace Berkelium

#endif // BERKELIUM_HOST_DELEGATE_HPP_
