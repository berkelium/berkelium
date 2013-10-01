var source={
"berkelium-cpp/src/demo/demos/demo1.c":
"// Copyright (c) 2012 The Berkelium Authors. All rights reserved.\n" +
"// Use of this source code is governed by a BSD-style license that can be\n" +
"// found in the LICENSE file.\n" +
"\n" +
"#include <berkelium.h>\n" +
"\n" +
"#include <stdio.h>\n" +
"\n" +
"/*\n" +
"void dumpProfile(Berkelium::LoggerRef logger, const std::string& name, Berkelium::ProfileRef profile)\n" +
"{\n" +
"\tif(!profile) {\n" +
"\t\tlogger->error() << name << \" profile not found!\" << std::endl;\n" +
"\t\treturn;\n" +
"\t}\n" +
"\tlogger->info() << \"profile application: \" << profile->getApplicationName() << std::endl;\n" +
"\tlogger->debug() << \"profile path: \" << profile->getProfilePath() << std::endl;\n" +
"\tlogger->debug() << \"profile is found: \" << profile->isFound() << std::endl;\n" +
"\tlogger->debug() << \"profile in use: \" << profile->isInUse() << std::endl;\n" +
"}\n" +
"*/\n" +
"\n" +
"/*\n" +
"void setupBkEnv(BK_Env* env);\n" +
"*/\n" +
"/*\n" +
"void logger(BK_Env* env, BK_LogDelegate self, BK_Runtime runtime,\n" +
"\t\tBK_LogSource source, BK_LogType type, bk_string clazz, bk_string name,\n" +
"\t\tbk_string message)\n" +
"{\n" +
"\tfprintf(stderr, \"[%s %s] %s\\n\", clazz, name, message);\n" +
"}\n" +
"\n" +
"void step1_1a_Runtime()\n" +
"{\n" +
"\tfprintf(stderr, \"1\\n\");\n" +
"\tBK_Runtime runtime = BK_BerkeliumFactory_createRuntime(NULL);\n" +
"\tfprintf(stderr, \"2\\n\");\n" +
"\tBK_Runtime_free(NULL, runtime);\n" +
"\tfprintf(stderr, \"3\\n\");\n" +
"}\n" +
"\n" +
"void step1_1b_Runtime()\n" +
"{\n" +
"\tfprintf(stderr, \"1\\n\");\n" +
"\tstruct _BK_LogDelegate delegate;\n" +
"\tdelegate.log = logger;\n" +
"\tfprintf(stderr, \"2\\n\");\n" +
"\tBK_Runtime runtime = BK_BerkeliumFactory_createRuntimeWithLog(NULL, &delegate);\n" +
"\tfprintf(stderr, \"3\\n\");\n" +
"\tBK_Runtime_free(NULL, runtime);\n" +
"\tfprintf(stderr, \"4\\n\");\n" +
"}\n" +
"\n" +
"void step1_2_HostExecutable()\n" +
"{\n" +
"\tfprintf(stderr, \"1\\n\");\n" +
"\tBK_Runtime runtime = BK_BerkeliumFactory_createRuntime(NULL);\n" +
"\tfprintf(stderr, \"2\\n\");\n" +
"\tstruct _BK_LogDelegate delegate;\n" +
"\tdelegate.log = &logger;\n" +
"\tfprintf(stderr, \"3\\n\");\n" +
"\tBK_Runtime runtime = BK_BerkeliumFactory_createRuntimeWithLog(NULL, &delegate);\n" +
"\n" +
"\tfprintf(stderr, \"4\\n\");\n" +
"\tBK_HostExecutable_free(NULL, exe);\n" +
"\tfprintf(stderr, \"5\\n\");\n" +
"\tBK_Runtime_free(NULL, runtime);\n" +
"\tfprintf(stderr, \"6\\n\");\n" +
"}\n" +
"*/\n" +
"int main(int argc, char* argv[])\n" +
"{\n" +
"\t/*\n" +
"\tfprintf(stderr, \"step1_1a\\n\");\n" +
"\tstep1_1a_Runtime();\n" +
"\n" +
"\tfprintf(stderr, \"step1_1b\\n\");\n" +
"\tstep1_1b_Runtime();\n" +
"\n" +
"\tfprintf(stderr, \"step1_2\\n\");\n" +
"\tstep1_2_HostExecutable();\n" +
"\t*/\n" +
"\n" +
"\n" +
"\t/*\n" +
"\tfprintf(stderr, \"1\\n\");\n" +
"\t*/\n" +
"\t/*\n" +
"\tBK_Runtime runtime = BK_BerkeliumFactory_createRuntime(NULL);\n" +
"\tfprintf(stderr, \"berkelium demo application...\\n\");\n" +
"\tBK_HostExecutable exe = BK_Runtime_forSystemInstalled(NULL, runtime);\n" +
"\t*/\n" +
"\t/*\n" +
"\tfprintf(stderr, \"2\\n\");\n" +
"\t*/\n" +
"\n" +
"\t/*\n" +
"\tBerkelium::HostExecutableRef host(runtime->forSystemInstalled());\n" +
"\n" +
"\tBerkelium::HostVersionRef version(host->getVersion());\n" +
"\tlogger->info() << \"host version string: \" << version->getVersionString() << std::endl;\n" +
"\n" +
"\tdumpProfile(logger, \"temporary\", runtime->createTemporaryProfile());\n" +
"\tdumpProfile(logger, \"Chrome\", runtime->getChromeProfile());\n" +
"\tdumpProfile(logger, \"Chromium\", runtime->getChromiumProfile());\n" +
"\tdumpProfile(logger, \"Berkelium\", runtime->forProfile(\"berkelium\"));\n" +
"\t*/\n" +
"\n" +
"\t/*\n" +
"\tBK_HostExecutable_free(NULL, exe);\n" +
"\t*/\n" +
"\t/*\n" +
"\tfprintf(stderr, \"3\\n\");\n" +
"\t*/\n" +
"\t/*\n" +
"\tBK_Runtime_free(NULL, runtime);\n" +
"\t*/\n" +
"\t/*\n" +
"\tfprintf(stderr, \"4\\n\");\n" +
"\t*/\n" +
"\n" +
"\treturn 0;\n" +
"}\n" +
"","berkelium-cpp/src/demo/demos/demo1.cpp":
"// Copyright (c) 2012 The Berkelium Authors. All rights reserved.\n" +
"// Use of this source code is governed by a BSD-style license that can be\n" +
"// found in the LICENSE file.\n" +
"\n" +
"#include <Berkelium/API/Runtime.hpp>\n" +
"#include <Berkelium/API/Util.hpp>\n" +
"#include <Berkelium/API/Profile.hpp>\n" +
"#include <Berkelium/API/HostVersion.hpp>\n" +
"#include <Berkelium/API/HostExecutable.hpp>\n" +
"#include <Berkelium/API/BerkeliumFactory.hpp>\n" +
"#include <Berkelium/API/Logger.hpp>\n" +
"\n" +
"void dumpProfile(Berkelium::LoggerRef logger, const std::string& name, Berkelium::ProfileRef profile)\n" +
"{\n" +
"\tif(!profile) {\n" +
"\t\tlogger->error() << name << \" profile not found!\" << std::endl;\n" +
"\t\treturn;\n" +
"\t}\n" +
"\tlogger->info() << \"profile application: \" << profile->getApplicationName() << std::endl;\n" +
"\tlogger->debug() << \"profile path: \" << profile->getProfilePath() << std::endl;\n" +
"\tlogger->debug() << \"profile is found: \" << profile->isFound() << std::endl;\n" +
"\tlogger->debug() << \"profile in use: \" << profile->isInUse() << std::endl;\n" +
"}\n" +
"\n" +
"int main(int argc, char* argv[])\n" +
"{\n" +
"\tBerkelium::RuntimeRef runtime(Berkelium::Util::createRuntime(argc, argv));\n" +
"\tBerkelium::LoggerRef logger(Berkelium::Util::createRootLogger(runtime));\n" +
"\n" +
"\tlogger->info() << \"berkelium demo application...\" << std::endl;\n" +
"\tBerkelium::HostExecutableRef host(runtime->forSystemInstalled());\n" +
"\n" +
"\tBerkelium::HostVersionRef version(host->getVersion());\n" +
"\tlogger->info() << \"host version string: \" << version->getVersionString() << std::endl;\n" +
"\n" +
"\tdumpProfile(logger, \"temporary\", runtime->createTemporaryProfile());\n" +
"\tdumpProfile(logger, \"Chrome\", runtime->getChromeProfile());\n" +
"\tdumpProfile(logger, \"Chromium\", runtime->getChromiumProfile());\n" +
"\tdumpProfile(logger, \"Berkelium\", runtime->forProfile(\"berkelium\"));\n" +
"}\n" +
"","berkelium-cpp/src/demo/demos/demo2.cpp":
"// Copyright (c) 2012 The Berkelium Authors. All rights reserved.\n" +
"// Use of this source code is governed by a BSD-style license that can be\n" +
"// found in the LICENSE file.\n" +
"\n" +
"#include <Berkelium/API/Runtime.hpp>\n" +
"#include <Berkelium/API/BerkeliumFactory.hpp>\n" +
"#include <Berkelium/API/Instance.hpp>\n" +
"#include <Berkelium/API/Util.hpp>\n" +
"#include <Berkelium/API/Window.hpp>\n" +
"#include <Berkelium/API/Tab.hpp>\n" +
"#include <Berkelium/API/Logger.hpp>\n" +
"\n" +
"int main(int argc, char* argv[])\n" +
"{\n" +
"\tBerkelium::RuntimeRef runtime(Berkelium::Util::createRuntime(argc, argv));\n" +
"\tBerkelium::LoggerRef logger(Berkelium::Util::createRootLogger(runtime));\n" +
"\n" +
"\tlogger->info() << \"berkelium demo application...\" << std::endl;\n" +
"\tBerkelium::HostExecutableRef host(runtime->forSystemInstalled());\n" +
"\tif(!host) {\n" +
"\t\tlogger->info() << \"berkelium host executable not found!\" << std::endl;\n" +
"\t\treturn 1;\n" +
"\t}\n" +
"\tBerkelium::ProfileRef profile(runtime->createTemporaryProfile());\n" +
"\tlogger->info() << \"starting berkelium browser...\" << std::endl;\n" +
"\tBerkelium::InstanceRef instance(runtime->open(host, profile));\n" +
"\tif(!instance) {\n" +
"\t\tlogger->info() << \"berkelium browser can not be started!\" << std::endl;\n" +
"\t\treturn 1;\n" +
"\t}\n" +
"\tlogger->info() << \"berkelium browser is running!\" << std::endl;\n" +
"\n" +
"\tBerkelium::WindowRef win(instance->createWindow(false));\n" +
"\tBerkelium::TabRef tab(win->createTab());\n" +
"\n" +
"\tlogger->info() << \"waiting 10s...\" << std::endl;\n" +
"\n" +
"\tfor(int i = 0; i < 10000; i += 100) {\n" +
"\t\truntime->update(100);\n" +
"\t}\n" +
"\n" +
"\tlogger->info() << \"shutting down browser...\" << std::endl;\n" +
"\tinstance.reset();\n" +
"\tprofile.reset();\n" +
"\thost.reset();\n" +
"\tlogger->info() << \"done!\" << std::endl;\n" +
"}\n" +
"","berkelium-cpp/src/demo/examples/logger1.c":
"// Copyright (c) 2013 The Berkelium Authors. All rights reserved.\n" +
"// Use of this source code is governed by a BSD-style license that can be\n" +
"// found in the LICENSE file.\n" +
"\n" +
"#include <berkelium.h>\n" +
"#include <stdio.h>\n" +
"#include <string.h>\n" +
"#include <stdlib.h>\n" +
"\n" +
"void log_delegate(BK_Env* env, BK_LogDelegate self, BK_Runtime runtime, BK_LogSource source, BK_LogType type,\n" +
"\t\tbk_string clazz, bk_string name, bk_string message)\n" +
"{\n" +
"\t// just a demo: write log message to System.err...\n" +
"\t/*GREEN*/fprintf(stderr, \"%s %s %s\\n\", clazz, name, message);/*GREEN*/\n" +
"}\n" +
"\n" +
"int main(int argc, char* argv[])\n" +
"{\n" +
"\tBK_LogDelegate log = (BK_LogDelegate)malloc(sizeof(struct _BK_LogDelegate));\n" +
"\tlog->log = log_delegate;\n" +
"\n" +
"\tBK_Runtime runtime = BK_BerkeliumFactory_createRuntimeWithLog(/*HINT:BK_ENV*/NULL/*HINT*/, log);\n" +
"\n" +
"\t/*BLUE*/// The Logger Code here is only necessary to demonstrate an invocation of the LogDelegate/*BLUE*/\n" +
"\t/*BLUE*/// Berkelium itself creates log messages that are delegated to the Log Delegate/*BLUE*/\n" +
"\t/*BLUE*/// Because of this it is not necessary to use the Logger Classes directly/*BLUE*/\n" +
"\tBK_Logger logger = BK_Runtime_getLogger(/*HINT:BK_ENV*/NULL/*HINT*/, runtime, strdup(\"logger1\"), strdup(\"\"));\n" +
"\t// Create a Log Message to demonstrate the LogDelegate\n" +
"\tBK_Logger_info(/*HINT:BK_ENV*/NULL/*HINT*/, logger, strdup(\"Hello World!\"));\n" +
"\n" +
"\tBK_Logger_free(/*HINT:BK_ENV*/NULL/*HINT*/, logger);\n" +
"\n" +
"\tBK_Runtime_free(/*HINT:BK_ENV*/NULL/*HINT*/, runtime);\n" +
"\n" +
"\treturn 0;\n" +
"}\n" +
"","berkelium-cpp/src/demo/examples/logger1.cpp":
"// Copyright (c) 2013 The Berkelium Authors. All rights reserved.\n" +
"// Use of this source code is governed by a BSD-style license that can be\n" +
"// found in the LICENSE file.\n" +
"\n" +
"#include <Berkelium/API/Runtime.hpp>\n" +
"#include <Berkelium/API/LogDelegate.hpp>\n" +
"#include <Berkelium/API/Logger.hpp>\n" +
"#include <Berkelium/API/BerkeliumFactory.hpp>\n" +
"\n" +
"class DemoLogDelegate : public Berkelium::LogDelegate\n" +
"{\n" +
"public:\n" +
"\tvirtual void log(Berkelium::RuntimeRef runtime, Berkelium::LogSource source, Berkelium::LogType type,\n" +
"\t\t\t const std::string& clazz, const std::string& name, const std::string& message) {\n" +
"\t\t// just a demo: write log message to std::cout...\n" +
"\t\t/*GREEN*/std::cout << clazz << ' ' << name << ' ' << message << std::endl;/*GREEN*/\n" +
"\t}\n" +
"};\n" +
"\n" +
"int main(int argc, char* argv[])\n" +
"{\n" +
"\t// This LogDelegate will receive all Log Messages from Berkelium\n" +
"\tBerkelium::LogDelegateRef log(/*GREEN*/new DemoLogDelegate()/*GREEN*/);\n" +
"\t// Create Runtime, use given Log Delegate\n" +
"\tBerkelium::RuntimeRef runtime(/*GREEN*/Berkelium::BerkeliumFactory::createRuntimeWithLog(log)/*GREEN*/);\n" +
"\n" +
"\t/*BLUE*/// The Logger Code here is only necessary to demonstrate an invocation of the LogDelegate/*BLUE*/\n" +
"\t/*BLUE*/// Berkelium itself creates log messages that are delegated to the Log Delegate/*BLUE*/\n" +
"\t/*BLUE*/// Because of this it is not necessary to use the Logger Classes directly/*BLUE*/\n" +
"\tBerkelium::LoggerRef logger = runtime->getLogger(\"logger1\", \"\");\n" +
"\t// Create a Log Message to demonstrate the LogDelegate\n" +
"\tlogger->info(\"Hello World!\");\n" +
"\n" +
"}\n" +
"","berkelium-cpp/src/demo/examples/runtime1.c":
"// Copyright (c) 2013 The Berkelium Authors. All rights reserved.\n" +
"// Use of this source code is governed by a BSD-style license that can be\n" +
"// found in the LICENSE file.\n" +
"\n" +
"#include <berkelium.h>\n" +
"#include <stdio.h>\n" +
"\n" +
"int main(int argc, char* argv[])\n" +
"{\n" +
"\t/*GREEN*/// Create Runtime/*GREEN*/\n" +
"\tBK_Runtime runtime = /*GREEN*/BK_BerkeliumFactory_createRuntime(/*HINT:BK_ENV*/NULL/*HINT*/)/*GREEN*/;\n" +
"\tBK_Runtime_free(/*HINT:BK_ENV*/NULL/*HINT*/, runtime);\n" +
"\n" +
"\treturn 0;\n" +
"}\n" +
"","berkelium-cpp/src/demo/examples/runtime1.cpp":
"// Copyright (c) 2013 The Berkelium Authors. All rights reserved.\n" +
"// Use of this source code is governed by a BSD-style license that can be\n" +
"// found in the LICENSE file.\n" +
"\n" +
"#include <Berkelium/API/Runtime.hpp>\n" +
"#include <Berkelium/API/BerkeliumFactory.hpp>\n" +
"\n" +
"int main(int argc, char* argv[])\n" +
"{\n" +
"\t/*GREEN*/// Create Runtime/*GREEN*/\n" +
"\tBerkelium::RuntimeRef runtime(/*GREEN*/Berkelium::BerkeliumFactory::createRuntime()/*GREEN*/);\n" +
"}\n" +
"","berkelium-cpp/src/demo/examples/runtime2.c":
"// Copyright (c) 2013 The Berkelium Authors. All rights reserved.\n" +
"// Use of this source code is governed by a BSD-style license that can be\n" +
"// found in the LICENSE file.\n" +
"\n" +
"#include <berkelium.h>\n" +
"#include <stdio.h>\n" +
"\n" +
"int main(int argc, char* argv[])\n" +
"{\n" +
"\tBK_Runtime runtime = BK_BerkeliumFactory_createRuntime(/*HINT:BK_ENV*/NULL/*HINT*/);\n" +
"\n" +
"\t/*GREEN*/// Runtime will be freed here/*GREEN*/\n" +
"\t/*GREEN*/BK_Runtime_free(/*HINT:BK_ENV*/NULL/*HINT*/, runtime);/*GREEN*/\n" +
"\n" +
"\treturn 0;\n" +
"}\n" +
"","berkelium-cpp/src/demo/examples/runtime2.cpp":
"// Copyright (c) 2013 The Berkelium Authors. All rights reserved.\n" +
"// Use of this source code is governed by a BSD-style license that can be\n" +
"// found in the LICENSE file.\n" +
"\n" +
"#include <Berkelium/API/Runtime.hpp>\n" +
"#include <Berkelium/API/BerkeliumFactory.hpp>\n" +
"\n" +
"int main(int argc, char* argv[])\n" +
"{\n" +
"\t/*GREEN*/// no Runtime Instance yet/*GREEN*/\n" +
"\t{\n" +
"\t\tBerkelium::RuntimeRef runtime(Berkelium::BerkeliumFactory::createRuntime());\n" +
"\t\t/*GREEN*/// you can use the 'runtime' Variable here/*GREEN*/\n" +
"\t}\n" +
"\t/*GREEN*/// The 'runtime' Variable got out of scope - Runtime Instance was automaticly freed/*GREEN*/\n" +
"}\n" +
"","berkelium-cpp/src/demo/pngrenderer/pngrenderer.cpp":
"// Copyright (c) 2012 The Berkelium Authors. All rights reserved.\n" +
"// Use of this source code is governed by a BSD-style license that can be\n" +
"// found in the LICENSE file.\n" +
"\n" +
"#include <Berkelium/API/Runtime.hpp>\n" +
"#include <Berkelium/API/Profile.hpp>\n" +
"#include <Berkelium/API/HostExecutable.hpp>\n" +
"#include <Berkelium/API/BerkeliumFactory.hpp>\n" +
"#include <Berkelium/API/Instance.hpp>\n" +
"#include <Berkelium/API/Util.hpp>\n" +
"#include <Berkelium/API/Window.hpp>\n" +
"#include <Berkelium/API/Logger.hpp>\n" +
"\n" +
"using Berkelium::BerkeliumFactory;\n" +
"using Berkelium::HostExecutableRef;\n" +
"using Berkelium::ProfileRef;\n" +
"using Berkelium::InstanceRef;\n" +
"using Berkelium::WindowRef;\n" +
"\n" +
"int main(int argc, char* argv[])\n" +
"{\n" +
"\tBerkelium::RuntimeRef runtime(Berkelium::Util::createRuntime(argc, argv));\n" +
"\tBerkelium::LoggerRef logger(Berkelium::Util::createRootLogger(runtime));\n" +
"\n" +
"\tlogger->info() << \"running pngrenderer...\" << std::endl;\n" +
"\tHostExecutableRef host(runtime->forSystemInstalled());\n" +
"\n" +
"\tlogger->debug() << \"creating profile...\" << std::endl;\n" +
"\tProfileRef profile(runtime->createTemporaryProfile());\n" +
"\n" +
"\tlogger->debug() << \"launching berkelium host executable...\" << std::endl;\n" +
"\tInstanceRef instance(runtime->open(host, profile));\n" +
"\n" +
"\tlogger->debug() << \"creating window...\" << std::endl;\n" +
"\tWindowRef window = instance->createWindow(false);\n" +
"\n" +
"\tlogger->debug() << \"window: \" << window.get() << std::endl;\n" +
"\n" +
"\twindow->getTabCount();\n" +
"\n" +
"\tlogger->debug() << \"tested!\" << std::endl;\n" +
"\n" +
"\tfor(int i = 0; i < 2000; i += 100) {\n" +
"\t\truntime->update(100);\n" +
"\t}\n" +
"\n" +
"\tlogger->debug() << \"closing window...\" << std::endl;\n" +
"\twindow.reset();\n" +
"\n" +
"\tlogger->debug() << \"closing berkelium host executable...\" << std::endl;\n" +
"\tinstance.reset();\n" +
"\n" +
"\tlogger->debug() << \"closing profile...\" << std::endl;\n" +
"\tprofile.reset();\n" +
"\n" +
"\tlogger->debug() << \"closing host...\" << std::endl;\n" +
"\thost.reset();\n" +
"\n" +
"\tlogger->info() << \"pngrenderer finished!\" << std::endl;\n" +
"}\n" +
"","berkelium-java/src/test/java/org/berkelium/demo/demos/Demo1.java":
"package org.berkelium.demo.demos;\n" +
"\n" +
"import org.berkelium.api.BerkeliumFactory;\n" +
"import org.berkelium.api.HostExecutable;\n" +
"import org.berkelium.api.HostVersion;\n" +
"import org.berkelium.api.Runtime;\n" +
"import org.berkelium.api.Logger;\n" +
"import org.berkelium.api.Profile;\n" +
"import org.berkelium.api.Util;\n" +
"\n" +
"public class Demo1 {\n" +
"\tpublic static void dumpProfile(Logger logger, String name, Profile profile) {\n" +
"\t\tif(profile == null) {\n" +
"\t\t\tlogger.error(name + \" profile not found!\");\n" +
"\t\t\treturn;\n" +
"\t\t}\n" +
"\t\tlogger.info(\"profile application: \" + profile.getApplicationName());\n" +
"\t\tlogger.debug(\"profile path: \" + profile.getProfilePath());\n" +
"\t\tlogger.debug(\"profile is found: \" + profile.isFound());\n" +
"\t\tlogger.debug(\"profile in use: \" + profile.isInUse());\n" +
"\n" +
"\t\tprofile.dispose();\n" +
"\t}\n" +
"\n" +
"\tpublic static void main(String[] args) {\n" +
"\t\tRuntime runtime = BerkeliumFactory.getInstance().createRuntime();\n" +
"\n" +
"\t\tLogger logger = Util.createRootLogger(runtime);\n" +
"\n" +
"\t\tlogger.info(\"berkelium demo application...\");\n" +
"\t\tHostExecutable host = runtime.forSystemInstalled();\n" +
"\n" +
"\t\tHostVersion version = host.getVersion();\n" +
"\t\thost.dispose();\n" +
"\n" +
"\t\tlogger.info(\"host version string: \" + version);\n" +
"\t\tversion.dispose();\n" +
"\n" +
"\t\tdumpProfile(logger, \"temporary\", runtime.createTemporaryProfile());\n" +
"\t\tdumpProfile(logger, \"Chrome\", runtime.getChromeProfile());\n" +
"\t\tdumpProfile(logger, \"Chromium\", runtime.getChromiumProfile());\n" +
"\t\tdumpProfile(logger, \"Berkelium\", runtime.forProfile(\"berkelium\"));\n" +
"\t\truntime.dispose();\n" +
"\t}\n" +
"}\n" +
"","berkelium-java/src/test/java/org/berkelium/demo/demos/Demo2.java":
"package org.berkelium.demo.demos;\n" +
"\n" +
"import org.berkelium.api.BerkeliumFactory;\n" +
"import org.berkelium.api.HostExecutable;\n" +
"import org.berkelium.api.Instance;\n" +
"import org.berkelium.api.LogDelegate;\n" +
"import org.berkelium.api.LogSource;\n" +
"import org.berkelium.api.LogType;\n" +
"import org.berkelium.api.Runtime;\n" +
"import org.berkelium.api.Logger;\n" +
"import org.berkelium.api.Profile;\n" +
"import org.berkelium.api.Tab;\n" +
"import org.berkelium.api.Util;\n" +
"import org.berkelium.api.Window;\n" +
"\n" +
"public class Demo2 {\n" +
"\tprivate static LogDelegate log = new LogDelegate() {\n" +
"\t\t@Override\n" +
"\t\tpublic void log(Runtime runtime, LogSource source, LogType type,\n" +
"\t\t\t\tString clazz, String name, String message) {\n" +
"\t\t\tSystem.err.printf(\"%s %s %s %s %s\\n\", type, source, clazz, name, message);\n" +
"\t\t}\n" +
"\t};\n" +
"\n" +
"\tpublic static void main(String[] args) throws InterruptedException {\n" +
"\t\tRuntime runtime = BerkeliumFactory.getInstance().createRuntimeWithLog(log);\n" +
"\n" +
"\t\tLogger logger = runtime.getLogger(Demo2.class.getName(), \"\");\n" +
"\n" +
"\t\tlogger.info(\"berkelium demo application...\");\n" +
"\t\tHostExecutable host = runtime.forSystemInstalled();\n" +
"\n" +
"\t\tif(host == null) {\n" +
"\t\t\tlogger.error(\"berkelium host executable not found!\");\n" +
"\t\t\treturn;\n" +
"\t\t}\n" +
"\n" +
"\t\tProfile profile = runtime.createTemporaryProfile();\n" +
"\t\tlogger.info(\"starting berkelium browser...\");\n" +
"\n" +
"\t\tInstance instance = runtime.open(host, profile);\n" +
"\t\tif(instance == null) {\n" +
"\t\t\tlogger.info(\"berkelium browser can not be started!\");\n" +
"\t\t\treturn;\n" +
"\t\t}\n" +
"\t\tlogger.info(\"berkelium browser is running!\");\n" +
"\n" +
"\t\tWindow win = instance.createWindow(false);\n" +
"\t\tTab tab = win.createTab();\n" +
"\n" +
"\t\tlogger.info(\"waiting 10s...\");\n" +
"\n" +
"\t\tfor(int i = 0; i < 10000; i += 100) {\n" +
"\t\t\truntime.update(100);\n" +
"\t\t}\n" +
"\n" +
"\t\tlogger.info(\"shutting down browser...\");\n" +
"\n" +
"\t\tinstance.close();\n" +
"\n" +
"\t\tlogger.info(\"dispose tab...\");\n" +
"\t\ttab.dispose();\n" +
"\n" +
"\t\tlogger.info(\"dispose window...\");\n" +
"\t\twin.dispose();\n" +
"\n" +
"\t\tlogger.info(\"dispose instance...\");\n" +
"\t\tinstance.dispose();\n" +
"\n" +
"\t\tlogger.info(\"dispose profile...\");\n" +
"\t\tprofile.dispose();\n" +
"\n" +
"\t\tlogger.info(\"dispose host...\");\n" +
"\t\thost.dispose();\n" +
"\n" +
"\t\tlogger.info(\"dispose runtime...\");\n" +
"\t\truntime.dispose();\n" +
"\n" +
"\t\tlogger.info(\"disposed!\");\n" +
"\n" +
"\t\tlogger.dispose();\n" +
"\t}\n" +
"}\n" +
"","berkelium-java/src/test/java/org/berkelium/demo/examples/Logger1.java":
"// Copyright (c) 2013 The Berkelium Authors. All rights reserved.\n" +
"// Use of this source code is governed by a BSD-style license that can be\n" +
"// found in the LICENSE file.\n" +
"\n" +
"package org.berkelium.demo.examples;\n" +
"\n" +
"import org.berkelium.api.BerkeliumFactory;\n" +
"import org.berkelium.api.LogDelegate;\n" +
"import org.berkelium.api.LogSource;\n" +
"import org.berkelium.api.LogType;\n" +
"import org.berkelium.api.Logger;\n" +
"import org.berkelium.api.Runtime;\n" +
"\n" +
"public class Logger1 {\n" +
"\t// This LogDelegate will receive all Log Messages from Berkelium\n" +
"\tprivate static /*GREEN*/LogDelegate log/*GREEN*/ = new LogDelegate() {\n" +
"\t\t@Override\n" +
"\t\tpublic void log(Runtime runtime, LogSource source, LogType type,\n" +
"\t\t\tString clazz, String name, String message) {\n" +
"\t\t\t// just a demo: write log message to System.err...\n" +
"\t\t\t/*GREEN*/System.err.printf(\"%s %s %s\\n\", clazz, name, message);/*GREEN*/\n" +
"\t\t}\n" +
"\t};\n" +
"\n" +
"\tpublic static void main(String[] args) {\n" +
"\t\t// Create Runtime, use given Log Delegate\n" +
"\t\tRuntime runtime = /*GREEN*/BerkeliumFactory.getInstance().createRuntimeWithLog(log);/*GREEN*/\n" +
"\n" +
"\t\t/*BLUE*/// The Logger Code here is only necessary to demonstrate an invocation of the LogDelegate/*BLUE*/\n" +
"\t\t/*BLUE*/// Berkelium itself creates log messages that are delegated to the Log Delegate/*BLUE*/\n" +
"\t\t/*BLUE*/// Because of this it is not necessary to use the Logger Classes directly/*BLUE*/\n" +
"\t\tLogger logger = runtime.getLogger(Logger1.class.getName(), \"\");\n" +
"\t\t// Create a Log Message to demonstrate the LogDelegate\n" +
"\t\tlogger.info(\"Hello World!\");\n" +
"\n" +
"\t\t// close logger\n" +
"\t\tlogger.dispose();\n" +
"\n" +
"\t\t// close runtime\n" +
"\t\truntime.dispose();\n" +
"\t}\n" +
"}\n" +
"","berkelium-java/src/test/java/org/berkelium/demo/examples/Runtime1.java":
"// Copyright (c) 2013 The Berkelium Authors. All rights reserved.\n" +
"// Use of this source code is governed by a BSD-style license that can be\n" +
"// found in the LICENSE file.\n" +
"\n" +
"package org.berkelium.demo.examples;\n" +
"\n" +
"import org.berkelium.api.BerkeliumFactory;\n" +
"import org.berkelium.api.Runtime;\n" +
"\n" +
"public class Runtime1 {\n" +
"\tpublic static void main(String[] args) {\n" +
"\t\t/*GREEN*/// Create Runtime/*GREEN*/\n" +
"\t\tRuntime runtime = /*GREEN*/BerkeliumFactory.getInstance().createRuntime()/*GREEN*/;\n" +
"\t\t// close runtime\n" +
"\t\truntime.dispose();\n" +
"\t}\n" +
"}\n" +
"","berkelium-java/src/test/java/org/berkelium/demo/examples/Runtime2.java":
"// Copyright (c) 2013 The Berkelium Authors. All rights reserved.\n" +
"// Use of this source code is governed by a BSD-style license that can be\n" +
"// found in the LICENSE file.\n" +
"\n" +
"package org.berkelium.demo.examples;\n" +
"\n" +
"import org.berkelium.api.BerkeliumFactory;\n" +
"import org.berkelium.api.Runtime;\n" +
"\n" +
"public class Runtime2 {\n" +
"\tpublic static void main(String[] args) {\n" +
"\t\t// Create Runtime\n" +
"\t\tRuntime runtime = BerkeliumFactory.getInstance().createRuntime();\n" +
"\t\t/*GREEN*/// close runtime/*GREEN*/\n" +
"\t\t/*GREEN*/runtime.dispose();/*GREEN*/\n" +
"\t}\n" +
"}\n" +
""
}
