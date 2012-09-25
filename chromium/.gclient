solutions = [
  { "name"        : "deps",
    "url"         : "https://src.chromium.org/chrome/releases/22.0.1229.79",
    "deps_file"   : "DEPS",
    "managed"     : True,
    "custom_deps" : {
        "src/third_party/WebKit/LayoutTests": None,
        "src/chrome/tools/test/reference_build/chrome_win": None,
        "src/chrome_frame/tools/test/reference_build/chrome_win": None,
        "src/chrome/tools/test/reference_build/chrome_linux": None,
        "src/chrome/tools/test/reference_build/chrome_mac": None,
        "src/third_party/hunspell_dictionaries": None,
    },
    "safesync_url": "",
  },
]
