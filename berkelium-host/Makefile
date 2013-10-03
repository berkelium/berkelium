SOURCES2 = $(shell find src -name '*.cpp')
SOURCES = $(SOURCES2:src/%=%)

all: run/berkelium

.PHONY: clean
clean:
	rm -rf out run

out:
	$(eval Debug :=  $(shell test -d ../chromium/src/out/Debug && echo "true" || echo false))
	$(eval Release :=  $(shell test -d ../chromium/src/out/Release && echo "true" || echo false))
	@test ${Debug}${Release} != truetrue || (echo "Found chromium Debug and Release builds!" ; false)
	@test ${Debug}${Release} != falsefalse || (echo "Can not find chromium Debug or Release builds!" ; false)
	mkdir -p out/o/berkelium
	mkdir -p out/o/libberkelium
	test ${Debug} = false || ln -s ../../chromium/src/out/Debug out/chromium
	test ${Release} = false || ln -s ../../chromium/src/out/Release out/chromium
	# TODO just a hack...
	ln -s chromium out/Debug
	ln -s chromium out/Release

out/o/%.o: src/%.cpp src/*/*.hpp out
	g++ -I../berkelium-cpp/src/host/shared -I../berkelium-cpp/include -fexceptions -std=c++0x -DBERKELIUM_CPP_IMPL `pkg-config --cflags glib-2.0` `pkg-config --cflags gtk+-2.0` -Isrc/include -I../berkelium-cpp/include -I../chromium/src/third_party/skia/include/config -I../chromium/src -I../chromium/src/third_party/WebKit/Source/Platform/chromium -I../chromium/src/third_party/skia/include/core -D_DEBUG -DDEBUG -g -Wall -pthread -fno-exceptions -fno-strict-aliasing -Wno-unused-parameter -Wno-missing-field-initializers -fvisibility=hidden -pipe -fPIC -O0 -g -fno-rtti -fno-threadsafe-statics -fvisibility-inlines-hidden -MMD -MF $@.d.raw -c -o $@ $<

run/berkelium: $(SOURCES:%.cpp=out/o/%.o)
	test -d run || mkdir run
	for i in chrome.pak chrome_100_percent.pak resources.pak locales nacl_helper_bootstrap nacl_helper nacl_irt_x86_64.nexe; do test -e run/$$i || ln -s ../out/Debug/$$i run ; done
	`cat linux-link.txt | sed 's#-o out/Debug/chrome #-o run/berkelium $^ -L../build/out -lhost-shared -lberkelium-cpp #' | sed 's#out/Debug/obj.target/chrome/chrome/app/chrome_main.o##'`
