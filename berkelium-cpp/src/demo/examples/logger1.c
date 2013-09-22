// Copyright (c) 2013 The Berkelium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <berkelium.h>
#include <stdio.h>

int main(int argc, char* argv[])
{
	BK_Runtime runtime = BK_BerkeliumFactory_createRuntime(/*HINT:BK_ENV*/NULL/*HINT*/);

	/*GREEN*/// Runtime will be freed here/*GREEN*/
	/*GREEN*/BK_Runtime_free(/*HINT:BK_ENV*/NULL/*HINT*/, runtime);/*GREEN*/

	return 0;
}
