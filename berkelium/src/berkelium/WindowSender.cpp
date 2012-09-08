// Copyright (c) 2012 The Berkelium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "WindowSender.hpp"
#include "Berkelium.hpp"
#include "ChromiumPacketWriter.hpp"

#include "../include/IpcConstants.hpp"
#include "../include/PacketWriter.hpp"

#include "base/utf_string_conversions.h"
#include "content/public/browser/render_process_host.h"

#include <stdio.h>

namespace Berkelium {

WindowSender::WindowSender(content::RenderProcessHost* process) : process(process) {
	Berkelium::send("new Window");
}

WindowSender::~WindowSender() {
	Berkelium::send("delete Window");
}

bool WindowSender::OnMessageReceived(const IPC::Message& msg) {
	bool msg_is_ok = true;
	IPC_BEGIN_MESSAGE_MAP_EX(WindowSender, msg, msg_is_ok)
	IPC_MESSAGE_HANDLER(ViewHostMsg_UpdateRect, OnMsgUpdateRect)
	IPC_MESSAGE_HANDLER(ViewHostMsg_UpdateTitle, OnMsgUpdateTitle)
	IPC_MESSAGE_HANDLER(ViewHostMsg_ContextMenu, OnMsgContextMenu)
	IPC_END_MESSAGE_MAP_EX()
	;
	return false;
}

void WindowSender::OnMsgUpdateRect(const ViewHostMsg_UpdateRect_Params& params) {
	Berkelium::send("OnMsgUpdateRect");
	PacketWriter w(this, IpcConstants::Delegate::onPaint);
	for (std::vector<gfx::Rect>::const_iterator it = params.copy_rects.begin(); it != params.copy_rects.end(); ++it) {
		ChromiumPacketWriter::write(w, *it);
	}
}

void WindowSender::OnMsgUpdateTitle(int32 page_id, string16 title, WebKit::WebTextDirection) {
	Berkelium::send("OnMsgUpdateTitle");
	std::string t = UTF16ToUTF8(title);
	Berkelium::send(t.c_str());
	//window_delegate.onTitleChanged((Window*)this, UTF16ToUTF8(title));
}

void WindowSender::OnMsgContextMenu(const content::ContextMenuParams& params) {
	Berkelium::send("OnMsgContextMenu");
}

} // namespace Berkelium
