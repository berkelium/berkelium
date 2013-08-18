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

void setupBkEnv(BK_Env* env);

int main(int argc, char* argv[])
{
	BK_Env env;
	setupBkEnv(&env);

	/*
	fprintf(stderr, "1\n");
	*/
	BK_Runtime runtime = BK_BerkeliumFactory_createRuntime(&env);
	fprintf(stderr, "berkelium demo application...\n");
	BK_HostExecutable exe = BK_Runtime_forSystemInstalled(&env, runtime);
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

	BK_HostExecutable_free(&env, exe);
	/*
	fprintf(stderr, "3\n");
	*/
	BK_Runtime_free(&env, runtime);
	/*
	fprintf(stderr, "4\n");
	*/

	return 0;
}
