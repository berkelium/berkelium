// Copyright (c) 2012 The Berkelium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "WindowSender.hpp"
#include "Berkelium.hpp"

#include "../include/IpcConstants.hpp"

#include "base/utf_string_conversions.h"
#include "ui/surface/transport_dib.h"
#include "content/public/browser/render_process_host.h"

#include <stdio.h>

namespace Berkelium {

WindowSender::WindowSender(content::RenderProcessHost* process, WindowActions* actions) :
	actions(actions),
	process(process),
	channelId(-1)
{
}

WindowSender::~WindowSender() {
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
	/*
	TransportDIB* dib = process->GetTransportDIB(params.bitmap);
	PacketWriter* pw = Berkelium::getPacketWriter();
	pw->add_16(IpcConstants::Delegate::onPaint);
	/ *
	w.add(params.copy_rects .sourceBuffer);
	* /
	ChromiumPacketWriter::write(*pw, params.copy_rects);
	/ *
	pw->add_8(params.copy_rects.size());
	for (std::vector<gfx::Rect>::const_iterator it = rects.begin(); it != rects.end(); ++it) {
		ChromiumPacketWriter::write(w, *it);
	}
	* /
	const gfx::Rect& r = params.bitmap_rect;
	ChromiumPacketWriter::write(*pw, r);
	pw->add_data32(r.width() * r.height() * 4, dib->memory());

	/ *
	w.add(sourceBufferRect);
	w.add(params.copy_rects);
	w.add16(dx);
	w.add16(dy);
	w.add(scrollRect);
	* /
	pw->send();
	*/
}

void WindowSender::OnMsgUpdateTitle(int32 page_id, string16 title, WebKit::WebTextDirection) {
	/*
	PacketWriter* pw = Berkelium::getPacketWriter();
	pw->add_16(IpcConstants::Delegate::onTitleChanged);
	pw->add_str(UTF16ToUTF8(title));
	pw->send();
	*/
}

void WindowSender::OnMsgContextMenu(const content::ContextMenuParams& params) {
}

} // namespace Berkelium
