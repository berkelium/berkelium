// Copyright (c) 2013 The Berkelium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Berkelium/API/Runtime.hpp>
#include <Berkelium/API/BerkeliumFactory.hpp>

int main(int argc, char* argv[])
{
	/*GREEN*/// Create Runtime/*GREEN*/
	Berkelium::RuntimeRef runtime(/*GREEN*/Berkelium::BerkeliumFactory::createRuntime()/*GREEN*/);
}
