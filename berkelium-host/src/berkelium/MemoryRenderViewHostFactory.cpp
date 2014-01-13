// Copyright (c) 2012 The Berkelium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Berkelium/API/Logger.hpp>

#include "BerkeliumHost.hpp"
#include "MemoryRenderViewHostFactory.hpp"

#include "base/command_line.h"
#include "base/strings/utf_string_conversions.h"
#include "content/common/view_messages.h"
#include "content/browser/renderer_host/render_view_host_impl.h"

#include <Berkelium/IPC/Channel.hpp>
#include <Berkelium/IPC/Message.hpp>

#include "gurl.h"

#include <stdio.h>

#if defined(OS_WIN)
#pragma warning(disable : 4250)
#endif

namespace Berkelium {

using Ipc::Channel;
using Ipc::ChannelRef;
using Ipc::Message;
using Ipc::MessageRef;

extern LoggerRef logger;

class MemoryRenderViewHost : public content::RenderViewHostImpl {
private:
	content::SiteInstance* instance;
	content::RenderViewHostDelegate* delegate;
	content::RenderWidgetHostDelegate* widget_delegate;
	int routing_id;
	bool swapped_out;
	content::SessionStorageNamespace* session_storage_namespace;
	bool started;
	ChannelRef ipc;
	BerkeliumHostTabRef tab;

public:
	MemoryRenderViewHost(
		content::SiteInstance* instance,
		content::RenderViewHostDelegate* delegate,
		content::RenderWidgetHostDelegate* widget_delegate,
		int routing_id,
		int main_frame_routing_id,
		bool swapped_out,
		bool hidden)
	: content::RenderViewHostImpl(instance, delegate, widget_delegate, routing_id, main_frame_routing_id, swapped_out, hidden),
	instance(instance),
	delegate(delegate),
	widget_delegate(widget_delegate),
	routing_id(routing_id),
	swapped_out(swapped_out),
	session_storage_namespace(session_storage_namespace),
	started(false),
	tab()
	{
		fprintf(stderr, "new MemoryRenderViewHost! %p\n", this);
	}

	virtual ~MemoryRenderViewHost() {
		fprintf(stderr, "delete MemoryRenderViewHost!\n");
		BerkeliumHost::removeTab(this);
	}

	void dump(const char* text, const IPC::Message& msg) {
		return;
		uint32 type = msg.type();

#define dump_all_events 1
		/*
		Debug Code...

		use this line to show all events:
		*/
#define CHECK_EVENT(X) case X::ID: fprintf(stderr, "MemoryRenderViewHost::%s: " #X "\n", text); return
		/*
		and this line if want to only show other events:
#define CHECK_EVENT(X) case X::ID: break
		*/

		if(dump_all_events)
		switch(type) {
		CHECK_EVENT(ViewHostMsg_DocumentOnLoadCompletedInMainFrame);
		CHECK_EVENT(ViewHostMsg_UpdateEncoding);
		CHECK_EVENT(ViewHostMsg_DocumentLoadedInFrame);
		CHECK_EVENT(ViewHostMsg_FrameNavigate);
		CHECK_EVENT(ViewHostMsg_UpdateZoomLimits);
		CHECK_EVENT(ViewHostMsg_DidStartProvisionalLoadForFrame);
		CHECK_EVENT(ViewHostMsg_DidStartLoading);
		CHECK_EVENT(ViewHostMsg_DidStopLoading);
		CHECK_EVENT(ViewHostMsg_DidFinishLoad);
		CHECK_EVENT(ViewHostMsg_DidZoomURL);
		CHECK_EVENT(ViewHostMsg_HasTouchEventHandlers);
		CHECK_EVENT(ViewHostMsg_RenderViewReady);
		CHECK_EVENT(ViewHostMsg_DidChangeNumWheelEvents);
		CHECK_EVENT(ViewHostMsg_WillInsertBody);
		CHECK_EVENT(ViewHostMsg_DidChangeLoadProgress);
		CHECK_EVENT(ViewHostMsg_DocumentAvailableInMainFrame);
		CHECK_EVENT(ViewHostMsg_WebUISend);
		CHECK_EVENT(ViewHostMsg_UpdateState);
		CHECK_EVENT(ViewHostMsg_UpdateScreenRects_ACK);
		CHECK_EVENT(ViewHostMsg_UpdateTitle);
		CHECK_EVENT(ViewHostMsg_RenderProcessGone);
		CHECK_EVENT(ViewHostMsg_SetTooltipText);
		CHECK_EVENT(ViewHostMsg_UpdateRect);
		CHECK_EVENT(ViewHostMsg_DidFirstVisuallyNonEmptyPaint);
		CHECK_EVENT(ViewMsg_UpdateRect_ACK);
		CHECK_EVENT(ViewMsg_MoveOrResizeStarted);
		CHECK_EVENT(ViewMsg_Resize);
		CHECK_EVENT(ViewMsg_ChangeResizeRect);
		CHECK_EVENT(ViewMsg_New);
		CHECK_EVENT(ViewMsg_AllowBindings);
		CHECK_EVENT(ViewMsg_SetAltErrorPageURL);
		CHECK_EVENT(ViewMsg_Navigate);
		CHECK_EVENT(ViewMsg_SetWebUIProperty);
		CHECK_EVENT(ViewMsg_UpdateWebPreferences);
		CHECK_EVENT(ViewMsg_UpdateScreenRects);
		CHECK_EVENT(ViewMsg_WasShown);
		CHECK_EVENT(ViewMsg_WasHidden);
		CHECK_EVENT(ViewMsg_Close);
		CHECK_EVENT(ViewMsg_ScriptEvalRequest);

		default:
			uint32 group = type >> 16;
			uint32 code = type & 0xFFFF;
			const char* groupstr;
			if(group == ViewMsgStart) {
				groupstr = "ViewMsg";
			} else if(group == InputMsgStart) {
				groupstr = "InputMsg";
			} else if(group == AutofillMsgStart) {
				groupstr = "AutofillMsg";
			} else if(group == ChromeMsgStart) {
				groupstr = "ChromeMsg";
			} else if(group == ExtensionMsgStart) {
				groupstr = "ExtensionMsg";
			} else if(group == ImageMsgStart) {
				groupstr = "ImageMsg";
			} else if(group == PrintMsgStart) {
				groupstr = "PrintMsg";
			} else {
				groupstr = NULL;
			}
			if(groupstr == NULL) {
				fprintf(stderr, "MemoryRenderViewHost::%s: %d %d\n", text, group, code);
			} else {
				fprintf(stderr, "MemoryRenderViewHost::%s: %s %d\n", text, groupstr, code);
			}
		}
	}

	virtual bool OnMessageReceived(const IPC::Message& msg) {
		dump("Recv2", msg);
		uint32 type = msg.type();
		/*
		if(type == 0x010000 + 2272) {
			fprintf(stderr, "**** test!!!!\n");
			OnRenderViewReady();
			//fprintf(stderr, "**** view: \n", view_);
		}
		*/

		if(!started) {
			if(type == ViewHostMsg_DidStopLoading::ID) {
				started = true;
				OnReady();
			}
		} else {
			IPC_BEGIN_MESSAGE_MAP(MemoryRenderViewHost, msg)
			IPC_MESSAGE_HANDLER_GENERIC(ViewHostMsg_DidStartLoading, OnLoading(true))
			IPC_MESSAGE_HANDLER_GENERIC(ViewHostMsg_DidStopLoading, OnLoading(false))
			//IPC_MESSAGE_HANDLER(ViewHostMsg_DidFirstVisuallyNonEmptyPaint, OnUpdateRect)
			IPC_MESSAGE_HANDLER(ViewHostMsg_UpdateRect, OnUpdateRect)
			IPC_END_MESSAGE_MAP()
		}

		// after processing them with berkelium,
		// just pass them to chrome
		return content::RenderViewHostImpl::OnMessageReceived(msg);
	}

	virtual bool Send(IPC::Message* msg) {
		if(msg == NULL) {
			fprintf(stderr, "Send NULL Message!\n");
		} else {
			dump("Send2", *msg);
		}
		return content::RenderViewHostImpl::Send(msg);
	}

	void setBerkeliumHostTabRef(BerkeliumHostTabRef t) {
		tab = t;
	}

	virtual void OnUpdateRect(const ViewHostMsg_UpdateRect_Params& params) {
		/*
		IPC_STRUCT_MEMBER(std::vector<gfx::Rect>, copy_rects)
		IPC_STRUCT_MEMBER(std::vector<content::WebPluginGeometry>, plugin_window_moves)
		IPC_STRUCT_MEMBER(ui::LatencyInfo, latency_info)
		*/
		fprintf(stderr, "*** OnPaint()\n");
#if defined(TOOLKIT_GTK)
		fprintf(stderr, "Bitmap %d\n", params.bitmap.shmkey);
#else
#error TODO
#endif
		if(tab) {
			tab->sendOnPaint();
		} else {
			logger->error("OnReady: no BerkeliumHostTabRef found!");
		}
		fprintf(stderr, "Flags %d\n", params.flags);
		fprintf(stderr, "NeedsAck %d\n", params.needs_ack);
		fprintf(stderr, "ScaleFactor %f\n", params.scale_factor);
		fprintf(stderr, "ScrollDelta %d %d\n", params.scroll_delta.x(), params.scroll_delta.y());
		fprintf(stderr, "ScrollOffset %d %d\n", params.scroll_offset.x(), params.scroll_offset.y());
		fprintf(stderr, "ViewSize %d %d\n", params.view_size.width(), params.view_size.height());
		fprintf(stderr, "BitmapRect(%d %d %d %d)\n", params.bitmap_rect.x(),params.bitmap_rect.y(),params.bitmap_rect.width(),params.bitmap_rect.height());
		fprintf(stderr, "ScrollRect(%d %d %d %d)\n", params.scroll_rect.x(),params.scroll_rect.y(),params.scroll_rect.width(),params.scroll_rect.height());
		if(tab) {
			tab->sendOnPaint();
		} else {
			logger->error("OnPaint: no BerkeliumHostTabRef found!");
		}
	}

	virtual void CloseTab() {
		content::RenderViewHostImpl::ClosePage();
	}

	virtual void NavigateTo(const std::string& url) {
		NavigateToURL(GURL(url));
	}

	virtual void Update() {
		/*
		if(!ipc->isEmpty()) {
			ipc->recv(msg);
			std::string str = msg->get_str();

			if(str.compare("Navigate") == 0) {
				NavigateTo(msg->get_str());
			} else {
				fprintf(stderr, "*** Update(%s)\n", str.c_str());
			}
		}
		*/
		fprintf(stderr, "*** Update\n");
	}

	virtual void OnReady() {
		/*
		msg->reset();
		msg->add_str("OnReady");
		ipc->send(msg);
		*/
		fprintf(stderr, "*** OnReady()\n");
		if(tab) {
			tab->sendOnReady();
		} else {
			logger->error("OnReady: no BerkeliumHostTabRef found!");
		}
	}

	virtual void OnLoading(bool start) {
		/*
		msg->reset();
		msg->add_str("OnLoading");
		msg->add_8(start);
		ipc->send(msg);
		*/
		fprintf(stderr, "*** OnLoading(%s)\n", start ? "true" : "false");
	}
};

void setBerkeliumHostTabRef(content::RenderViewHost* rvh, BerkeliumHostTabRef tab)
{
	MemoryRenderViewHost* mrvh((MemoryRenderViewHost*)rvh);
	mrvh->setBerkeliumHostTabRef(tab);
	//mrvh->WasShown();
}

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
	const std::string debug("berkelium-debug");
	const std::string bs("berkelium");
	const std::string ds("user-data-dir");

	if(CommandLine::ForCurrentProcess()->HasSwitch(debug)) {
		return !!BerkeliumHost::initDebug(CommandLine::ForCurrentProcess()->GetSwitchValueASCII(debug));
	}

	if(!CommandLine::ForCurrentProcess()->HasSwitch(bs)) return false;
	if(!CommandLine::ForCurrentProcess()->HasSwitch(ds)) return false;
	std::string id = CommandLine::ForCurrentProcess()->GetSwitchValueASCII(bs);
	std::string dir = CommandLine::ForCurrentProcess()->GetSwitchValueASCII(ds);
	if(id.empty() || dir.empty()) {
		return false;
	}
	return !!BerkeliumHost::init(dir, id);
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
		RenderViewHostFactory::UnregisterFactory();
	}
}

content::RenderViewHost* MemoryRenderViewHostFactory::CreateRenderViewHost(
	content::SiteInstance* instance,
	content::RenderViewHostDelegate* delegate,
	content::RenderWidgetHostDelegate* widget_delegate,
	int routing_id,
	int main_frame_routing_id,
	bool swapped_out) {

	BerkeliumHost::lasyInit();

	bool hidden = true;

	return new MemoryRenderViewHost(instance, delegate, widget_delegate, routing_id,
			main_frame_routing_id, swapped_out, hidden);
}

} // namespace Berkelium
