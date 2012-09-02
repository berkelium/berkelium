// Copyright (c) 2012 The Berkelium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "Berkelium.hpp"
#include "MemoryRenderViewHostFactory.hpp"
#include "WindowDelegate.hpp"

#include "base/utf_string_conversions.h"
#include "content/browser/renderer_host/render_view_host_impl.h"

#include "googleurl/src/gurl.h"

#include <stdio.h>

namespace Berkelium {

class MemoryRenderViewHost : public content::RenderViewHostImpl {
private:
	scoped_ptr<Window> window;
	WindowDelegate window_delegate;

	content::SiteInstance* instance;
	content::RenderViewHostDelegate* delegate;
	content::RenderWidgetHostDelegate* widget_delegate;
	int routing_id;
	bool swapped_out;
	content::SessionStorageNamespace* session_storage_namespace;

public:
	MemoryRenderViewHost(
		content::SiteInstance* instance,
		content::RenderViewHostDelegate* delegate,
		content::RenderWidgetHostDelegate* widget_delegate,
		int routing_id,
		bool swapped_out,
		content::SessionStorageNamespace* session_storage_namespace)
	: content::RenderViewHostImpl(instance, delegate, widget_delegate, routing_id, swapped_out, session_storage_namespace),
	window(new Window()),
	instance(instance),
	delegate(delegate),
	widget_delegate(widget_delegate),
	routing_id(routing_id),
	swapped_out(swapped_out),
	session_storage_namespace(session_storage_namespace)
	{
	}

	virtual ~MemoryRenderViewHost() {
	}

	virtual bool OnMessageReceived(const IPC::Message& msg) {
		// pass all events to berkelium
		window->OnMessageReceived(msg);

		// after processing them with berkelium,
		// just pass them to chrome
		return content::RenderViewHostImpl::OnMessageReceived(msg);
	}
};

bool doRegister(int argc, const char** argv) {
	bool ret = false;
	for(int i = 0; i < argc; i++) {
		//fprintf(stderr, "main: %i : '%s'\n", i, argv[i]);
		if(strcmp("--berkelium", argv[i]) == 0) {
			ret = true;
		}
	}
	//fprintf(stderr, "register factory: %s\n", ret ? "true" : "false");
	return ret;
}

MemoryRenderViewHostFactory::MemoryRenderViewHostFactory(int argc, const char** argv)
: registered(doRegister(argc, argv)) {
	if(registered) {
		// do not call Berkelium::init() here,
		// Chromium is not yet correctly initialised
		RenderViewHostFactory::RegisterFactory(this);
	}
}

MemoryRenderViewHostFactory::~MemoryRenderViewHostFactory() {
	if(registered) {
		Berkelium::destory();
		RenderViewHostFactory::UnregisterFactory();
	}
}

content::RenderViewHost* MemoryRenderViewHostFactory::CreateRenderViewHost(
	content::SiteInstance* instance,
	content::RenderViewHostDelegate* delegate,
	content::RenderWidgetHostDelegate* widget_delegate,
	int routing_id,
	bool swapped_out,
	content::SessionStorageNamespace* session_storage) {

	// lasy init berkelium here
	Berkelium::init();

	return new MemoryRenderViewHost(instance, delegate, widget_delegate, routing_id,
			swapped_out, session_storage);
}

} // namespace Berkelium
