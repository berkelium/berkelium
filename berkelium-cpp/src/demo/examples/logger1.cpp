// Copyright (c) 2013 The Berkelium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Berkelium/API/Runtime.hpp>
#include <Berkelium/API/LogDelegate.hpp>
#include <Berkelium/API/Logger.hpp>
#include <Berkelium/API/BerkeliumFactory.hpp>

class DemoLogDelegate : public Berkelium::LogDelegate
{
public:
	virtual void log(Berkelium::RuntimeRef runtime, Berkelium::LogSource source, Berkelium::LogType type,
			 const std::string& clazz, const std::string& name, const std::string& message) {
		// just a demo: write log message to std::cout...
		/*GREEN*/std::cout << clazz << ' ' << name << ' ' << message << std::endl;/*GREEN*/
	}
};

int main(int argc, char* argv[])
{
	// This LogDelegate will receive all Log Messages from Berkelium
	Berkelium::LogDelegateRef log(/*GREEN*/new DemoLogDelegate()/*GREEN*/);
	// Create Runtime, use given Log Delegate
	Berkelium::RuntimeRef runtime(/*GREEN*/Berkelium::BerkeliumFactory::createRuntimeWithLog(log)/*GREEN*/);

	/*BLUE*/// The Logger Code here is only necessary to demonstrate an invocation of the LogDelegate/*BLUE*/
	/*BLUE*/// Berkelium itself creates log messages that are delegated to the Log Delegate/*BLUE*/
	/*BLUE*/// Because of this it is not necessary to use the Logger Classes directly/*BLUE*/
	Berkelium::LoggerRef logger = runtime->getLogger("logger1", "");
	// Create a Log Message to demonstrate the LogDelegate
	logger->info("Hello World!");

}
