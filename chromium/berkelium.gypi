# Copyright (c) 2013 The Berkelium Authors. All rights reserved.
# Use of this source code is governed by a BSD-style license that can be
# found in the LICENSE file.
{
  'targets': [{
    'target_name': 'berkelium-library',
    'type': 'none',
    'conditions': [
      ['OS=="linux"', {
        'link_settings': {
          'library_dirs': [
             '../build/out',
           ],
          'libraries': [
            '-lhost-shared',
            '-lberkelium-cpp',
          ],
        },
      }, {
      }],
    ],
  }, {
    'target_name': 'berkelium',
    'type': 'executable',
    'sources': [
      '../berkelium-host/src/berkelium/BerkeliumChromiumDelegate.cpp',
      '../berkelium-host/src/berkelium/BerkeliumMain.cpp',
      '../berkelium-host/src/berkelium/BrowserWindow.cpp',
      '../berkelium-host/src/berkelium/MemoryRenderViewHostFactory.cpp',
      '../berkelium-host/src/berkelium/MemoryRenderViewHostFactory.hpp',
      '../berkelium-host/src/berkelium/Rect.hpp',
      'src/chrome/app/chrome_dll_resource.h',
      'src/chrome/app/chrome_main_delegate.cc',
      'src/chrome/app/chrome_main_delegate.h',
    ],
    'defines': [
      'BERKELIUM_CPP_IMPL',
    ],
    'include_dirs': [
      '../berkelium-cpp/src/host/shared',
      '../berkelium-cpp/include',
      'src/url',
    ],
    'dependencies': [
      'berkelium-library',
      '../skia/skia.gyp:skia',
      'chrome',
      '<@(chromium_browser_dependencies)',
      '<@(chromium_child_dependencies)',
      '../content/content.gyp:content_app_both',
    ],
    'msvs_settings': {
      'VCLinkerTool': {
        'SubSystem': '2',         # Set /SUBSYSTEM:WINDOWS
        'AdditionalLibraryDirectories': [
		  '<(DEPTH)/../../build/out/Debug',
		],
        'AdditionalDependencies': [
		  'berkelium-cpp.lib',
		  'host-shared.lib',
		],
      },
    },
    'conditions': [
      ['OS=="linux"', {
        'dependencies': [
          '../build/linux/system.gyp:gtk',
        ],
        'cflags': [
          '-std=c++0x',
        ],
        'sources': [
          'src/chrome/app/chrome_exe_main_gtk.cc',
        ],
      }, {
      }],
      ['OS=="linux" and linux_use_tcmalloc==1', {
        'dependencies': [
          '<(allocator_target)',
        ],
      }, {
      }],
      ['OS=="win"', {
        'sources': [
          'src/content/app/startup_helper_win.cc',
          'src/chrome/app/chrome_dll.rc',
          '<(SHARED_INTERMEDIATE_DIR)/chrome_version/chrome_exe_version.rc',
        ],
        'dependencies': [
          'chrome_dll',
          'chrome_nacl_win64',
          'chrome_process_finder',
          'chrome_version_resources',
          'installer_util',
          'image_pre_reader',
          '../base/base.gyp:base',
          '../breakpad/breakpad.gyp:breakpad_handler',
          '../breakpad/breakpad.gyp:breakpad_sender',
          '../components/components.gyp:breakpad_component',
          '../sandbox/sandbox.gyp:sandbox',
          'app/policy/cloud_policy_codegen.gyp:policy',
        ],
      }, {
      }],
    ],
  }],
}
