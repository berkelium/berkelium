// Copyright (c) 2012 The Berkelium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "Berkelium.hpp"

#include "content/public/browser/browser_thread.h"

#include <stdio.h>

namespace Berkelium {

void update() {
	//fprintf(stderr, "berkelium update!\n");
	content::BrowserThread::PostDelayedTask(content::BrowserThread::UI, FROM_HERE, base::Bind(&update), 100);

}

void Berkelium::init() {
	static bool initialised = false;
	if(initialised) return;
	initialised = true;

	fprintf(stderr, "berkelium started!\n");
	update();
}

void Berkelium::destory() {
	fprintf(stderr, "berkelium closed!\n");
}

void Berkelium::send(std::string msg) {
	fprintf(stderr, "berkelium send: %s\n", msg.c_str());
}

} // namespace Berkelium
