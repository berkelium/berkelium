// Copyright (c) 2014 The Berkelium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "content/browser/web_contents/web_contents_impl.h"
#include "content/port/browser/web_contents_view_port.h"
#include "content/port/browser/render_view_host_delegate_view.h"
#include "content/public/browser/web_contents_view_delegate.h"
#include "content/public/browser/render_widget_host_view.h"

#include <iostream>

using namespace content;
using base::string16;

RenderWidgetHostView* BerkeliumCreateViewForWidget(RenderWidgetHost*);

#define X fprintf(stderr, "BerkeliumWebContentsView::%s\n", __func__)

class BerkeliumWebContentsView: public WebContentsViewPort,
		public RenderViewHostDelegateView {
private:
	WebContentsImpl* web_contents_;
	gfx::Size requested_size_;

public:
	BerkeliumWebContentsView(WebContentsImpl* web_contents) :
			WebContentsViewPort(), //
			RenderViewHostDelegateView(), //
			web_contents_(web_contents), //
			requested_size_() {
		X;
	}

	virtual ~BerkeliumWebContentsView() {
		X;
	}

	/////////////////////////////////
	// WebContentsViewPort
	virtual void CreateView(const gfx::Size& initial_size,
			gfx::NativeView context) {
		X;
		fprintf(stderr, "CreateView %d %d\n", initial_size.width(),
				initial_size.height());
		requested_size_ = initial_size;
	}

	virtual RenderWidgetHostView* CreateViewForWidget(
			RenderWidgetHost* render_widget_host) {
		X;
		return BerkeliumCreateViewForWidget(render_widget_host);
	}

	virtual RenderWidgetHostView* CreateViewForPopupWidget(
			RenderWidgetHost* render_widget_host) {
		X;
		return BerkeliumCreateViewForWidget(render_widget_host);
	}

	virtual void SetPageTitle(const string16& title) {
		X;
		std::cerr << title << std::endl;
	}

	virtual void RenderViewCreated(RenderViewHost* host) {
		X;
	}

	virtual void RenderViewSwappedIn(RenderViewHost* host) {
		X;
	}

	virtual void SetOverscrollControllerEnabled(bool enabled) {
		X;
	}

#if defined(OS_MACOSX)
	virtual bool IsEventTracking() const {
		X;
		return false;
	}

	virtual void CloseTabAfterEventTracking() {
		X;
	}
#endif

	/////////////////////////////////
	// WebContentsView
	virtual gfx::NativeView GetNativeView() const {
		X;
		return NULL;
	}

	virtual gfx::NativeView GetContentNativeView() const {
		X;
		return NULL;
	}

	virtual gfx::NativeWindow GetTopLevelNativeWindow() const {
		X;
		return NULL;
	}

	virtual void GetContainerBounds(gfx::Rect* out) const {
		X;
		fprintf(stderr, "GetContainerBounds %d %d\n", requested_size_.width(),
				requested_size_.height());
		out->set_x(0);
		out->set_y(0);
		out->set_size(requested_size_);
	}

	virtual void OnTabCrashed(base::TerminationStatus status, int error_code) {
		X;
	}

	virtual void SizeContents(const gfx::Size& size) {
		X;
		requested_size_ = size;
		fprintf(stderr, "SizeContents %d %d\n", size.width(), size.height());
		RenderWidgetHostView* rwhv = web_contents_->GetRenderWidgetHostView();
		if (rwhv) {
			rwhv->SetSize(size);
		}
	}

	virtual void Focus() {
		X;
	}

	virtual void SetInitialFocus() {
		X;
	}

	virtual void StoreFocus() {
		X;
	}

	virtual void RestoreFocus() {
		X;
	}

	virtual DropData* GetDropData() const {
		X;
		return NULL;
	}

	virtual gfx::Rect GetViewBounds() const {
		X;
		return gfx::Rect();
	}

#if defined(OS_MACOSX)
	virtual void SetAllowOverlappingViews(bool overlapping) {
		X;
	}

	virtual bool GetAllowOverlappingViews() const {
		X;
		return false;
	}
#endif

	/////////////////////////////////
	// RenderViewHostDelegateView

	virtual void ShowPopupMenu(const gfx::Rect& bounds, int item_height,
			double item_font_size, int selected_item,
			const std::vector<MenuItem>& items, bool right_aligned,
			bool allow_multiple_selection) {
		X;
	}
};

WebContentsViewPort* BerkeliumCreateWebContentsView(
		WebContentsImpl* web_contents, WebContentsViewDelegate* delegate,
		RenderViewHostDelegateView** render_view_host_delegate_view) {
	BerkeliumWebContentsView* ret(new BerkeliumWebContentsView(web_contents));

	*render_view_host_delegate_view = ret;
	return ret;
}
