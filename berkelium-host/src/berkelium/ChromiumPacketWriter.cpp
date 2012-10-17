// Copyright (c) 2012 The Berkelium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ChromiumPacketWriter.hpp"
#include "PacketWriter.hpp"

namespace Berkelium {

void ChromiumPacketWriter::write(PacketWriter& w, const gfx::Rect& rect) {
	w.add_16(rect.x());
	w.add_16(rect.y());
	w.add_16(rect.width());
	w.add_16(rect.height());
}

void ChromiumPacketWriter::write(PacketWriter& w, std::vector<gfx::Rect> rects) {
	w.add_8(rects.size());
	for (std::vector<gfx::Rect>::const_iterator it = rects.begin(); it != rects.end(); ++it) {
		ChromiumPacketWriter::write(w, *it);
	}
}

} // namespace Berkelium
