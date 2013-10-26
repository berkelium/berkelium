// Copyright (c) 2013 The Berkelium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <berkelium.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void log_delegate(BK_Env* env, BK_LogDelegate self, BK_Runtime runtime, BK_LogSource source, BK_LogType type,
		bk_string clazz, bk_string name, bk_string message)
{
	// just a demo: write log message to System.err...
	/*GREEN*/fprintf(stderr, "%s %s %s\n", clazz, name, message);/*GREEN*/
}

int main(int argc, char* argv[])
{
	BK_LogDelegate log = (BK_LogDelegate)malloc(sizeof(struct _BK_LogDelegate));

	BK_Runtime runtime = BK_BerkeliumFactory_createRuntimeWithLog(/*HINT:BK_ENV*/NULL/*HINT*/, log);

	/*BLUE*/// The Logger Code here is only necessary to demonstrate an invocation of the LogDelegate/*BLUE*/
	/*BLUE*/// Berkelium itself creates log messages that are delegated to the Log Delegate/*BLUE*/
	/*BLUE*/// Because of this it is not necessary to use the Logger Classes directly/*BLUE*/
	BK_Logger logger = BK_Runtime_getLogger(/*HINT:BK_ENV*/NULL/*HINT*/, runtime, strdup("logger1"), strdup(""));
    
	log->log = log_delegate;

	// Create a Log Message to demonstrate the LogDelegate
	BK_Logger_info(/*HINT:BK_ENV*/NULL/*HINT*/, logger, strdup("Hello World!"));

	BK_Logger_free(/*HINT:BK_ENV*/NULL/*HINT*/, logger);

	BK_Runtime_free(/*HINT:BK_ENV*/NULL/*HINT*/, runtime);

	return 0;
}
