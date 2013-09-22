// Copyright (c) 2013 The Berkelium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Berkelium/API/Runtime.hpp>
#include <Berkelium/API/BerkeliumFactory.hpp>

int main(int argc, char* argv[])
{
	/*GREEN*/// no Runtime Instance yet/*GREEN*/
	{
		Berkelium::RuntimeRef runtime(Berkelium::BerkeliumFactory::createRuntime());
		/*GREEN*/// you can use the 'runtime' Variable here/*GREEN*/
	}
	/*GREEN*/// The 'runtime' Variable got out of scope - Runtime Instance was automaticly freed/*GREEN*/
}
