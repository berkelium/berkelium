// Copyright (c) 2013 The Berkelium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <berkelium.h>
#include <stdio.h>

int main(int argc, char* argv[])
{
	/*GREEN*/// Create Runtime/*GREEN*/
	BK_Runtime runtime = /*GREEN*/BK_BerkeliumFactory_createRuntime(/*HINT:BK_ENV*/NULL/*HINT*/)/*GREEN*/;
	BK_Runtime_free(/*HINT:BK_ENV*/NULL/*HINT*/, runtime);

	return 0;
}
