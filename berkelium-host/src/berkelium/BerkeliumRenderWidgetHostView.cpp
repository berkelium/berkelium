// Copyright (c) 2014 The Berkelium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "content/public/browser/render_widget_host.h"
#include "content/browser/renderer_host/render_widget_host_impl.h"
#include "content/browser/renderer_host/render_widget_host_view_base.h"

using namespace content;

#define X fprintf(stderr, "BerkeliumRenderWidgetHostView::%s\n", __func__)

class BerkeliumRenderWidgetHostView: public content::RenderWidgetHostViewBase {
private:
	content::RenderWidgetHostImpl* host_;
	gfx::Rect bounds_;
	bool is_hidden_;

public:
	BerkeliumRenderWidgetHostView(content::RenderWidgetHost* host) :
			content::RenderWidgetHostViewBase(), //
			host_(RenderWidgetHostImpl::From(host)), //
			bounds_(), //
			is_hidden_(host_->is_hidden()) {
		X;
		host_->SetView(this);
	}

	virtual ~BerkeliumRenderWidgetHostView() {
		X;
	}

	/////////////////////////////////
	// RenderWidgetHostViewPort
	virtual void InitAsPopup(RenderWidgetHostView* parent_host_view,
			const gfx::Rect& pos) {
		X;
	}

	virtual void InitAsFullscreen(RenderWidgetHostView* reference_host_view) {
		X;
	}

	virtual void WasShown() {
		X;
		if (!host_ || !is_hidden_)
			return;

		is_hidden_ = false;

		host_->WasShown();
	}

	virtual void WasHidden() {
		X;
		if (!host_ || is_hidden_)
			return;
		is_hidden_ = true;
		host_->WasHidden();
	}

	virtual void MovePluginWindows(const gfx::Vector2d& scroll_offset,
			const std::vector<WebPluginGeometry>& moves) {
		X;
	}

	virtual void Blur() {
		X;
	}

	virtual void UpdateCursor(const WebCursor& cursor) {
		X;
	}

	virtual void SetIsLoading(bool is_loading) {
		X;
	}

	virtual void TextInputTypeChanged(ui::TextInputType type,
			ui::TextInputMode mode, bool can_compose_inline) {
		X;
	}

	virtual void ImeCancelComposition() {
		X;
	}

#if defined(OS_MACOSX) || defined(OS_WIN) || defined(USE_AURA)
	virtual void ImeCompositionRangeChanged(const gfx::Range& range, const std::vector<gfx::Rect>& character_bounds) {
		X;
	}
#endif

	virtual void DidUpdateBackingStore(const gfx::Rect& scroll_rect,
			const gfx::Vector2d& scroll_delta,
			const std::vector<gfx::Rect>& copy_rects,
			const ui::LatencyInfo& latency_info) {
		X;
	}

	virtual void RenderProcessGone(base::TerminationStatus status,
			int error_code) {
	}

	virtual void Destroy() {
		X;
	}

	virtual void SetTooltipText(const string16& tooltip_text) {
		X;
	}

	//virtual void SelectionChanged(const string16& text, size_t offset, const gfx::Range& range) {}

	virtual void SelectionBoundsChanged(
			const ViewHostMsg_SelectionBounds_Params& params) {
		X;
	}

	virtual void ScrollOffsetChanged() {
		X;
	}

	virtual BackingStore* AllocBackingStore(const gfx::Size& size) {
		X;
		return NULL;
	}

	virtual void CopyFromCompositingSurface(const gfx::Rect& src_subrect,
			const gfx::Size& dst_size,
			const base::Callback<void(bool, const SkBitmap&)>& callback) {
		X;
	}

	virtual void CopyFromCompositingSurfaceToVideoFrame(
			const gfx::Rect& src_subrect,
			const scoped_refptr<media::VideoFrame>& target,
			const base::Callback<void(bool)>& callback) {
		X;
	}

	virtual bool CanCopyToVideoFrame() const {
		X;
		return false;
	}

	//virtual bool CanSubscribeFrame() const {}

	//virtual void BeginFrameSubscription(scoped_ptr<RenderWidgetHostViewFrameSubscriber> subscriber) {}

	//virtual void EndFrameSubscription() {}

	virtual void OnAcceleratedCompositingStateChange() {
		X;
	}

	virtual void AcceleratedSurfaceBuffersSwapped(
			const GpuHostMsg_AcceleratedSurfaceBuffersSwapped_Params& params_in_pixel,
			int gpu_host_id) {
		X;
	}
	virtual void AcceleratedSurfacePostSubBuffer(
			const GpuHostMsg_AcceleratedSurfacePostSubBuffer_Params& params_in_pixel,
			int gpu_host_id) {
		X;
	}

	virtual void AcceleratedSurfaceSuspend() {
		X;
	}

	virtual void AcceleratedSurfaceRelease() {
		X;
	}

	virtual bool HasAcceleratedSurface(const gfx::Size& desired_size) {
		X;
		return false;
	}

	//virtual void OnSwapCompositorFrame(uint32 output_surface_id, scoped_ptr<cc::CompositorFrame> frame) {}

	virtual void GetScreenInfo(WebKit::WebScreenInfo* results) {
		X;
	}

	//virtual gfx::Size GetPhysicalBackingSize() const {}

	//virtual float GetOverdrawBottomHeight() const {}

	virtual gfx::Rect GetBoundsInRootWindow() {
		X;
		return gfx::Rect();
	}

	virtual gfx::GLSurfaceHandle GetCompositingSurface() {
		X;
		return gfx::GLSurfaceHandle();
	}

	//virtual void ProcessAckedTouchEvent(const TouchEventWithLatencyInfo& touch, InputEventAckState ack_result) {}

	//virtual SyntheticGesture* CreateSmoothScrollGesture(bool scroll_down, int pixels_to_scroll, int mouse_event_x, 	int mouse_event_y) {}

	//virtual SyntheticGesture* CreatePinchGesture(bool zoom_in, int pixels_to_move, int anchor_x, int anchor_y) {}

	virtual void SetHasHorizontalScrollbar(bool has_horizontal_scrollbar) {
		X;
	}

	virtual void SetScrollOffsetPinning(bool is_pinned_to_left,
			bool is_pinned_to_right) {
		X;
	}

	//virtual void UnhandledWheelEvent(const WebKit::WebMouseWheelEvent& event) {}

	//virtual InputEventAckState FilterInputEvent(const WebKit::WebInputEvent& input_event) {}

	//virtual void GestureEventAck(int gesture_event_type, InputEventAckState ack_result) {}

	//virtual void OnOverscrolled(gfx::Vector2dF accumulated_overscroll, gfx::Vector2dF current_fling_velocity) {}

	//virtual void SetPopupType(WebKit::WebPopupType popup_type) {}
	//virtual WebKit::WebPopupType GetPopupType() {}

	//virtual BrowserAccessibilityManager* GetBrowserAccessibilityManager() const {}

	virtual void OnAccessibilityEvents(
			const std::vector<AccessibilityHostMsg_EventParams>& params) {
		X;
	}

	//virtual uint32 RendererFrameNumber() {}
	//virtual void DidReceiveRendererFrame() {}

#if defined(OS_MACOSX)
	virtual void AboutToWaitForBackingStoreMsg() {
		X;
	}

	virtual bool PostProcessEventForPluginIme(const NativeWebKeyboardEvent& event) {
		X;
		return false;
	}
#endif

#if defined(OS_WIN) && !defined(USE_AURA)
	virtual void WillWmDestroy() {
		X;
	}
#endif

#if defined(OS_WIN) && defined(USE_AURA)
	virtual void SetParentNativeViewAccessible(gfx::NativeViewAccessible accessible_parent) {
		X;
	}
#endif

	/////////////////////////////////
	// RenderWidgetHostView
	virtual void InitAsChild(gfx::NativeView parent_view) {
		X;
	}

	virtual RenderWidgetHost* GetRenderWidgetHost() const {
		X;
		return NULL;
	}

	virtual void SetSize(const gfx::Size& size) {
		X;
		fprintf(stderr, "SetSize %d %d\n", size.width(), size.height());
		bounds_.set_size(size);
	}

	virtual void SetBounds(const gfx::Rect& rect) {
		X;
		fprintf(stderr, "SetBounds %d %d %d %d\n", rect.x(), rect.y(),
				rect.width(), rect.height());
		bounds_ = rect;
	}

	virtual gfx::NativeView GetNativeView() const {
		X;
		return gfx::NativeView();
	}

	virtual gfx::NativeViewId GetNativeViewId() const {
		X;
		return gfx::NativeViewId();
	}

	virtual gfx::NativeViewAccessible GetNativeViewAccessible() {
		X;
		return gfx::NativeViewAccessible();
	}

	virtual void Focus() {
		X;
	}

	virtual bool HasFocus() const {
		X;
		return false;
	}
	virtual bool IsSurfaceAvailableForCopy() const {
		X;
		return false;
	}

	virtual void Show() {
		X;
		WasShown();
	}

	virtual void Hide() {
		X;
		WasHidden();
	}

	virtual bool IsShowing() {
		X;
		return !is_hidden_;
	}

	virtual gfx::Rect GetViewBounds() const {
		X;
		fprintf(stderr, "GetViewBounds %d %d %d %d\n", bounds_.x(), bounds_.y(),
				bounds_.width(), bounds_.height());
		return bounds_;
	}

	//virtual bool IsShowingContextMenu() const {}

	//virtual void SetShowingContextMenu(bool showing) {}

	//virtual string16 GetSelectedText() const {}

	//virtual void SetBackground(const SkBitmap& background) {}
	//virtual const SkBitmap& GetBackground() {}

	virtual bool LockMouse() {
		X;
		return false;
	}

	virtual void UnlockMouse() {
		X;
	}
	//virtual bool IsMouseLocked() {}

#if defined(OS_MACOSX)
	virtual void SetActive(bool active) {
		X;
	}

	virtual void SetTakesFocusOnlyOnMouseDown(bool flag) {
		X;
	}

	virtual void SetWindowVisibility(bool visible) {
		X;
	}

	virtual void WindowFrameChanged() {
		X;
	}

	virtual void ShowDefinitionForSelection() {
		X;
	}

	virtual bool SupportsSpeech() const {
		X;
		return false;
	}

	virtual void SpeakSelection() {
		X;
	}

	virtual bool IsSpeaking() const {
		X;
		return false;
	}

	virtual void StopSpeaking() {
		X;
		return false;
	}
#endif  // defined(OS_MACOSX)

#if defined(TOOLKIT_GTK)
	virtual GdkEventButton* GetLastMouseDown() {
		X;
		return NULL;
	}
	virtual gfx::NativeView BuildInputMethodsGtkMenu() {
		X;
		return gfx::NativeView();
	}
#endif  // defined(TOOLKIT_GTK)

#if defined(OS_WIN) && !defined(USE_AURA)
	virtual void SetClickthroughRegion(SkRegion* region) {
		X;
	}
#endif
};

content::RenderWidgetHostView* BerkeliumCreateViewForWidget(
		content::RenderWidgetHost* widget) {
	return new BerkeliumRenderWidgetHostView(widget);
}
