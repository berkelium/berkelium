// Copyright (c) 2012 The Berkelium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "Berkelium.hpp"
#include "MemoryRenderViewHostFactory.hpp"
#include "WindowActions.hpp"
#include "WindowSender.hpp"

#include "base/command_line.h"
#include "base/utf_string_conversions.h"
#include "content/browser/renderer_host/render_view_host_impl.h"

#include "googleurl/src/gurl.h"

#include <stdio.h>

namespace Berkelium {

class MemoryRenderViewHost : public content::RenderViewHostImpl, WindowActions {
private:
	scoped_ptr<WindowSender> window;

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
	window(new WindowSender(GetProcess(), this)),
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

	virtual void Shutdown() {
		content::RenderViewHostImpl::Shutdown();
	}
};

std::vector<std::string> split(std::string l, char delim) {
    std::replace(l.begin(), l.end(), delim, ' ');
    std::istringstream stm(l);
    std::vector<std::string> tokens;
    for (;;) {
        std::string word;
        if (!(stm >> word)) break;
        tokens.push_back(word);
    }
    return tokens;
}

bool doRegister() {
	const std::string bs("berkelium");
	const std::string ds("user-data-dir");
	if(!CommandLine::ForCurrentProcess()->HasSwitch(bs)) return false;
	if(!CommandLine::ForCurrentProcess()->HasSwitch(ds)) return false;
	std::string id = CommandLine::ForCurrentProcess()->GetSwitchValueASCII(bs);
	std::string dir = CommandLine::ForCurrentProcess()->GetSwitchValueASCII(ds);
	if(id.empty() || dir.empty()) {
		return false;
	}
	return Berkelium::init(dir, id);
}

MemoryRenderViewHostFactory::MemoryRenderViewHostFactory()
: registered(doRegister())
{
	if(registered) {
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

	Berkelium::lasyInit();

	return new MemoryRenderViewHost(instance, delegate, widget_delegate, routing_id,
			swapped_out, session_storage);
}

} // namespace Berkelium
