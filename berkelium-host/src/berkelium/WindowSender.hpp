// Copyright (c) 2012 The Berkelium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef WINDOW_SENDER_H_
#define WINDOW_SENDER_H_
#pragma once

#include "content/common/view_messages.h"

namespace content {
class RenderProcessHost;
}

#include "../include/PacketWriter.hpp"

namespace Berkelium {

class WindowActions;
class IpcSender;

class WindowSender {
public:
	WindowSender(content::RenderProcessHost*, WindowActions*);
	virtual ~WindowSender();

	virtual bool OnMessageReceived(const IPC::Message& msg);

protected:
	void OnMsgUpdateRect(const ViewHostMsg_UpdateRect_Params& params);
	void OnMsgUpdateTitle(int32 page_id, string16 title, WebKit::WebTextDirection);
	void OnMsgContextMenu(const content::ContextMenuParams& params);

private:
	WindowActions* actions;
	content::RenderProcessHost* process;
	int channelId;

	WindowSender(const WindowSender&);
	void operator=(const WindowSender&);
};

} // namespace Berkelium

#endif // WINDOW_SENDER_H_
