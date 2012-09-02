// Copyright (c) 2012 The Berkelium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef WINDOW_H_
#define WINDOW_H_
#pragma once

#include "content/common/view_messages.h"

namespace Berkelium {

class Window {
public:
	Window();
	virtual ~Window();

	virtual bool OnMessageReceived(const IPC::Message& msg);

protected:
	void OnMsgUpdateRect(const ViewHostMsg_UpdateRect_Params& params);
	void OnMsgUpdateTitle(int32 page_id, string16 title, WebKit::WebTextDirection);
	void OnMsgContextMenu(const content::ContextMenuParams& params);

private:
	Window(const Window&);
	void operator=(const Window&);
};

} // namespace Berkelium

#endif // WINDOW_H_
