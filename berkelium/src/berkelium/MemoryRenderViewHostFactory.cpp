// Copyright (c) 2012 The Berkelium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "MemoryRenderViewHostFactory.hpp"
#include "WindowDelegate.hpp"

#include "base/utf_string_conversions.h"
#include "content/browser/renderer_host/render_view_host_impl.h"
#include "content/common/view_messages.h"

#include "googleurl/src/gurl.h"

#include <stdio.h>

namespace Berkelium {

class MemoryRenderViewHost : public content::RenderViewHostImpl {
private:
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

	void BerkeliumOnMsgUpdateRect(const ViewHostMsg_UpdateRect_Params& params) {
		window_delegate.onPaint(NULL, NULL, Rect(), 0, NULL, 0, 0, Rect());
	}

	void BerkeliumOnMsgUpdateTitle(int32 page_id, string16 title, WebKit::WebTextDirection) {
		window_delegate.onTitleChanged((Window*)this, UTF16ToUTF8(title));
	}

	void BerkeliumOnMsgContextMenu(const content::ContextMenuParams& params) {
		fprintf(stderr, "context menu\n");
	}

	virtual bool OnMessageReceived(const IPC::Message& msg) {
		// capture all interesting events
		bool msg_is_ok = true;
		IPC_BEGIN_MESSAGE_MAP_EX(MemoryRenderViewHost, msg, msg_is_ok)
		IPC_MESSAGE_HANDLER(ViewHostMsg_UpdateRect, BerkeliumOnMsgUpdateRect)
		IPC_MESSAGE_HANDLER(ViewHostMsg_UpdateTitle, BerkeliumOnMsgUpdateTitle)
		IPC_MESSAGE_HANDLER(ViewHostMsg_ContextMenu, BerkeliumOnMsgContextMenu)
		IPC_END_MESSAGE_MAP_EX()
		;

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
		fprintf(stderr, "berkelium started!\n");
		RenderViewHostFactory::RegisterFactory(this);
	}
}

MemoryRenderViewHostFactory::~MemoryRenderViewHostFactory() {
	if(registered) {
		fprintf(stderr, "berkelium closed!\n");
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

	return new MemoryRenderViewHost(instance, delegate, widget_delegate, routing_id,
			swapped_out, session_storage);
}

} // namespace Berkelium
