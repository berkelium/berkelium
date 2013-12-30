// Copyright (c) 2012 The Berkelium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

// This File contains a copy of https://src.chromium.org/chrome/branches/1650/src/chrome/app/chrome_main.cc
// This File contains a copy of https://src.chromium.org/chrome/branches/1650/src/chrome/app/chrome_exe_main_win.cc

#include "MemoryRenderViewHostFactory.hpp"

#include "base/command_line.h"

#if 1 // BERKELIUM: start of chrome_main.cc
// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "chrome/app/chrome_main_delegate.h"

#include "content/public/app/content_main.h"

#if defined(OS_WIN)
#include "base/win/win_util.h"

#define DLLEXPORT __declspec(dllexport)

// We use extern C for the prototype DLLEXPORT to avoid C++ name mangling.
extern "C" {
DLLEXPORT int __cdecl ChromeMain(HINSTANCE instance,
                                 sandbox::SandboxInterfaceInfo* sandbox_info);
}
#elif defined(OS_POSIX)
extern "C" {
__attribute__((visibility("default")))
int ChromeMain(int argc, const char** argv);
}
#endif

#if defined(OS_WIN)
DLLEXPORT int __cdecl ChromeMain(HINSTANCE instance,
                                 sandbox::SandboxInterfaceInfo* sandbox_info) {
  // The process should crash when going through abnormal termination.
  base::win::SetShouldCrashOnProcessDetach(true);
  base::win::SetAbortBehaviorForCrashReporting();
  ChromeMainDelegate chrome_main_delegate;
#if 1 // BERKELIUM PATCH: added
  Berkelium::MemoryRenderViewHostFactory factory;
#endif // BERKELIUM PATCH: end
  int rv = content::ContentMain(instance, sandbox_info, &chrome_main_delegate);
  base::win::SetShouldCrashOnProcessDetach(false);
  return rv;
#elif defined(OS_POSIX)
int ChromeMain(int argc, const char** argv) {
  ChromeMainDelegate chrome_main_delegate;
#if 1 // BERKELIUM PATCH: added
  CommandLine::Init(argc, argv); // already here needed for berkelium
  Berkelium::MemoryRenderViewHostFactory factory;
#endif // BERKELIUM PATCH: end
  return content::ContentMain(argc, argv, &chrome_main_delegate);
#endif
}

#endif // BERKELIUM: end of chrome_main.cc

#if defined(OS_WIN) // BERKELIUM: start of chrome_exe_main_win.cc
// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <windows.h>
#include <tchar.h>

#include <string>

#include "base/at_exit.h"
#include "base/command_line.h"
#include "base/files/file_path.h"
#include "base/lazy_instance.h"
#if 0 // BERKELIUM PATCH: removed
#include "chrome/app/chrome_breakpad_client.h"
#include "chrome/app/breakpad_win.h"
#include "chrome/app/client_util.h"
#include "chrome/app/metro_driver_win.h"
#endif // BERKELIUM PATCH: end
#include "chrome/browser/chrome_process_finder_win.h"
#include "chrome/browser/policy/policy_path_parser.h"
#include "chrome/common/chrome_paths_internal.h"
#include "chrome/common/chrome_switches.h"
#include "components/breakpad/breakpad_client.h"
#include "content/public/app/startup_helper_win.h"
#include "content/public/common/result_codes.h"
#include "sandbox/win/src/sandbox_factory.h"

namespace {

#if 0 // BERKELIUM PATCH: removed
base::LazyInstance<chrome::ChromeBreakpadClient>::Leaky
    g_chrome_breakpad_client = LAZY_INSTANCE_INITIALIZER;
#endif // BERKELIUM PATCH: end

int RunChrome(HINSTANCE instance) {
#if 0 // BERKELIUM PATCH: removed
  breakpad::SetBreakpadClient(g_chrome_breakpad_client.Pointer());

  bool exit_now = true;
  // We restarted because of a previous crash. Ask user if we should relaunch.
  if (ShowRestartDialogIfCrashed(&exit_now)) {
    if (exit_now)
      return content::RESULT_CODE_NORMAL_EXIT;
  }
#endif // BERKELIUM PATCH: end

  // Initialize the sandbox services.
  sandbox::SandboxInterfaceInfo sandbox_info = {0};
  content::InitializeSandboxInfo(&sandbox_info);

#if 1 // BERKELIUM PATCH: added
  return ChromeMain(instance, &sandbox_info);
#else // BERKELIUM PATCH: removed
  // Load and launch the chrome dll. *Everything* happens inside.
  MainDllLoader* loader = MakeMainDllLoader();
  int rc = loader->Launch(instance, &sandbox_info);
  loader->RelaunchChromeBrowserWithNewCommandLineIfNeeded();
  delete loader;
  return rc;
#endif // BERKELIUM PATCH: end
}

// List of switches that it's safe to rendezvous early with. Fast start should
// not be done if a command line contains a switch not in this set.
// Note this is currently stored as a list of two because it's probably faster
// to iterate over this small array than building a map for constant time
// lookups.
const char* const kFastStartSwitches[] = {
  switches::kProfileDirectory,
  switches::kShowAppList,
};

bool IsFastStartSwitch(const std::string& command_line_switch) {
  for (size_t i = 0; i < arraysize(kFastStartSwitches); ++i) {
    if (command_line_switch == kFastStartSwitches[i])
      return true;
  }
  return false;
}

bool ContainsNonFastStartFlag(const CommandLine& command_line) {
  const CommandLine::SwitchMap& switches = command_line.GetSwitches();
  if (switches.size() > arraysize(kFastStartSwitches))
    return true;
  for (CommandLine::SwitchMap::const_iterator it = switches.begin();
       it != switches.end(); ++it) {
    if (!IsFastStartSwitch(it->first))
      return true;
  }
  return false;
}

bool AttemptFastNotify(const CommandLine& command_line) {
  if (ContainsNonFastStartFlag(command_line))
    return false;

  base::FilePath user_data_dir;
  if (!chrome::GetDefaultUserDataDirectory(&user_data_dir))
    return false;
  policy::path_parser::CheckUserDataDirPolicy(&user_data_dir);

  HWND chrome = chrome::FindRunningChromeWindow(user_data_dir);
  if (!chrome)
    return false;
  return chrome::AttemptToNotifyRunningChrome(chrome, true) ==
      chrome::NOTIFY_SUCCESS;
}

}  // namespace

int APIENTRY wWinMain(HINSTANCE instance, HINSTANCE prev, wchar_t*, int) {
  // Initialize the commandline singleton from the environment.
  CommandLine::Init(0, NULL);
  // The exit manager is in charge of calling the dtors of singletons.
  base::AtExitManager exit_manager;

  if (AttemptFastNotify(*CommandLine::ForCurrentProcess()))
    return 0;

#if 0 // BERKELIUM PATCH: removed
  MetroDriver metro_driver;
  if (metro_driver.in_metro_mode())
    return metro_driver.RunInMetro(instance, &RunChrome);
  // Not in metro mode, proceed as normal.
#endif // BERKELIUM PATCH: end
  return RunChrome(instance);
}

#endif // BERKELIUM: end of chrome_exe_main_win.cc
