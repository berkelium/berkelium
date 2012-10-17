// Copyright (c) 2012 The Berkelium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROMIUMPACKETWRITER_HPP_
#define CHROMIUMPACKETWRITER_HPP_
#pragma once

#include "ui/gfx/rect.h"

#include <vector>

namespace Berkelium {

class PacketWriter;

class ChromiumPacketWriter {
public:
	static void write(PacketWriter&, const gfx::Rect&);
	static void write(PacketWriter&, std::vector<gfx::Rect>);

private:
	ChromiumPacketWriter();
	~ChromiumPacketWriter();
};

} // namespace Berkelium

#endif /* CHROMIUMPACKETWRITER_HPP_ */
