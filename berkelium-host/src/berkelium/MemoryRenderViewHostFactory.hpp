// Copyright (c) 2012 The Berkelium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef MEMORY_RENDER_VIEW_HOST_FACTORY_HPP_
#define MEMORY_RENDER_VIEW_HOST_FACTORY_HPP_
#pragma once

#include "content/browser/renderer_host/render_view_host_factory.h"

namespace content {
class SiteInstance;
class RenderViewHostDelegate;
class RenderProcessHostFactory;
class SessionStorageNamespace;
}

#include "BerkeliumHostTab.hpp"

namespace Berkelium {

void setBerkeliumHostTabRef(content::RenderViewHost*, BerkeliumHostTabRef tab);

class MemoryRenderViewHostFactory : public content::RenderViewHostFactory {
private:
	const bool registered;

public:
	MemoryRenderViewHostFactory();
	virtual ~MemoryRenderViewHostFactory();

	virtual content::RenderViewHost* CreateRenderViewHost(
		content::SiteInstance* instance,
		content::RenderViewHostDelegate* delegate,
		content::RenderWidgetHostDelegate* widget_delegate,
		int routing_id,
		int main_frame_routing_id,
		bool swapped_out);

private:
	DISALLOW_COPY_AND_ASSIGN(MemoryRenderViewHostFactory);
};

} // namespace Berkelium

#endif // MEMORY_RENDER_VIEW_HOST_FACTORY_HPP_
