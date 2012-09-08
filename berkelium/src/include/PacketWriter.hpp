// Copyright (c) 2012 The Berkelium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef PACKET_WRITER_HPP_
#define PACKET_WRITER_HPP_
#pragma once

namespace Berkelium {

struct Rect;

class PacketWriter {
public:
	PacketWriter(WindowSender*, int);

	PacketWriter& add(int8);

	PacketWriter& add(int16);

	PacketWriter& add(int32);

	PacketWriter& add(int64);

	PacketWriter& add(Rect);

	void send();
};

} // namespace Berkelium


#endif // PACKET_WRITER_HPP_
