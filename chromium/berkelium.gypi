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
    ],
    'cflags': [
      '-std=c++0x',
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
    'conditions': [
      ['OS=="linux"', {
        'dependencies': [
          '../build/linux/system.gyp:gtk',
        ],
        'sources': [
          'src/chrome/app/chrome_exe_main_gtk.cc',
          'src/chrome/app/chrome_dll_resource.h',
          'src/chrome/app/chrome_main_delegate.cc',
          'src/chrome/app/chrome_main_delegate.h',
        ],
      }, {
      }],
      ['linux_use_tcmalloc==1', {
        'dependencies': [
          '<(allocator_target)',
        ],
      }, {
      }],
      ['OS=="win"', {
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
