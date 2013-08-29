// Copyright (c) 2012 The Berkelium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <berkelium.h>

#include <stdio.h>

/*
void dumpProfile(Berkelium::LoggerRef logger, const std::string& name, Berkelium::ProfileRef profile)
{
	if(!profile) {
		logger->error() << name << " profile not found!" << std::endl;
		return;
	}
	logger->info() << "profile application: " << profile->getApplicationName() << std::endl;
	logger->debug() << "profile path: " << profile->getProfilePath() << std::endl;
	logger->debug() << "profile is found: " << profile->isFound() << std::endl;
	logger->debug() << "profile in use: " << profile->isInUse() << std::endl;
}
*/

/*
void setupBkEnv(BK_Env* env);
*/
/*
void logger(BK_Env* env, BK_LogDelegate self, BK_Runtime runtime,
		BK_LogSource source, BK_LogType type, bk_string clazz, bk_string name,
		bk_string message)
{
	fprintf(stderr, "[%s %s] %s\n", clazz, name, message);
}

void step1_1a_Runtime()
{
	fprintf(stderr, "1\n");
	BK_Runtime runtime = BK_BerkeliumFactory_createRuntime(NULL);
	fprintf(stderr, "2\n");
	BK_Runtime_free(NULL, runtime);
	fprintf(stderr, "3\n");
}

void step1_1b_Runtime()
{
	fprintf(stderr, "1\n");
	struct _BK_LogDelegate delegate;
	delegate.log = logger;
	fprintf(stderr, "2\n");
	BK_Runtime runtime = BK_BerkeliumFactory_createRuntimeWithLog(NULL, &delegate);
	fprintf(stderr, "3\n");
	BK_Runtime_free(NULL, runtime);
	fprintf(stderr, "4\n");
}

void step1_2_HostExecutable()
{
	fprintf(stderr, "1\n");
	BK_Runtime runtime = BK_BerkeliumFactory_createRuntime(NULL);
	fprintf(stderr, "2\n");
	struct _BK_LogDelegate delegate;
	delegate.log = &logger;
	fprintf(stderr, "3\n");
	BK_Runtime runtime = BK_BerkeliumFactory_createRuntimeWithLog(NULL, &delegate);

	fprintf(stderr, "4\n");
	BK_HostExecutable_free(NULL, exe);
	fprintf(stderr, "5\n");
	BK_Runtime_free(NULL, runtime);
	fprintf(stderr, "6\n");
}
*/
int main(int argc, char* argv[])
{
	/*
	fprintf(stderr, "step1_1a\n");
	step1_1a_Runtime();

	fprintf(stderr, "step1_1b\n");
	step1_1b_Runtime();

	fprintf(stderr, "step1_2\n");
	step1_2_HostExecutable();
	*/


	/*
	fprintf(stderr, "1\n");
	*/
	/*
	BK_Runtime runtime = BK_BerkeliumFactory_createRuntime(NULL);
	fprintf(stderr, "berkelium demo application...\n");
	BK_HostExecutable exe = BK_Runtime_forSystemInstalled(NULL, runtime);
	*/
	/*
	fprintf(stderr, "2\n");
	*/

	/*
	Berkelium::HostExecutableRef host(runtime->forSystemInstalled());

	Berkelium::HostVersionRef version(host->getVersion());
	logger->info() << "host version string: " << version->getVersionString() << std::endl;

	dumpProfile(logger, "temporary", runtime->createTemporaryProfile());
	dumpProfile(logger, "Chrome", runtime->getChromeProfile());
	dumpProfile(logger, "Chromium", runtime->getChromiumProfile());
	dumpProfile(logger, "Berkelium", runtime->forProfile("berkelium"));
	*/

	/*
	BK_HostExecutable_free(NULL, exe);
	*/
	/*
	fprintf(stderr, "3\n");
	*/
	/*
	BK_Runtime_free(NULL, runtime);
	*/
	/*
	fprintf(stderr, "4\n");
	*/

	return 0;
}
