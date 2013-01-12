LINK_ARGS = \
-pthread \
-Wl,-z,noexecstack \
-fPIC \
-Wl,--threads \
-Wl,--thread-count=4 \
-Bout/Debug/../../third_party/gold \
-pie \
-Lout/Debug \
-Wl,-uIsHeapProfilerRunning,-uProfilerStart \
-Wl,-u_Z21InitialMallocHook_NewPKvj,-u_Z22InitialMallocHook_MMapPKvS0_jiiix,-u_Z22InitialMallocHook_SbrkPKvi \
-Wl,-u_Z21InitialMallocHook_NewPKvm,-u_Z22InitialMallocHook_MMapPKvS0_miiil,-u_Z22InitialMallocHook_SbrkPKvl \
-Wl,-u_ZN15HeapLeakChecker12IgnoreObjectEPKv,-u_ZN15HeapLeakChecker14UnIgnoreObjectEPKv \
-Wl,--icf=none \
-Wl,-rpath=\$ORIGIN/lib.target/ \
-Wl,-rpath-link=\out/Debug/lib.target/

LINK_OBJS = \
out/Debug/obj.target/chrome/chrome/app/chrome_exe_main_gtk.o \
out/Debug/obj.target/chrome/chrome/app/chrome_main_delegate.o

LINK_LIBS_OWN = \
out/Debug/obj.target/base/allocator/liballocator.a \
out/Debug/obj.target/chrome/libcommon.a \
out/Debug/obj.target/chrome/libbrowser.a \
out/Debug/obj.target/chrome/libdebugger.a \
out/Debug/obj.target/chrome/libplugin.a \
out/Debug/obj.target/chrome/librenderer.a \
out/Debug/obj.target/chrome/libutility.a \
out/Debug/obj.target/sync/libsyncapi_core.a \
out/Debug/obj.target/printing/libprinting.so \
out/Debug/obj.target/chrome/libservice.a \
out/Debug/obj.target/chrome/libinstaller_util.a \
out/Debug/obj.target/sandbox/libsandbox.a \
out/Debug/obj.target/base/third_party/dynamic_annotations/libdynamic_annotations.a \
out/Debug/obj.target/chrome/libcommon_net.a \
out/Debug/obj.target/base/libbase.so \
out/Debug/obj.target/crypto/libcrcrypto.so \
out/Debug/obj.target/net/libnet.so \
out/Debug/obj.target/third_party/icu/libicui18n.so \
out/Debug/obj.target/third_party/icu/libicuuc.so \
out/Debug/obj.target/gpu/libgpu.so \
out/Debug/obj.target/net/third_party/nss/libssl.a \
out/Debug/obj.target/third_party/zlib/libchrome_zlib.a \
out/Debug/obj.target/chrome/libmetrics_proto.a \
out/Debug/obj.target/third_party/protobuf/libprotobuf_lite.a \
out/Debug/obj.target/base/libbase_i18n.so \
out/Debug/obj.target/base/libbase_static.a \
out/Debug/obj.target/build/temp_gyp/libgoogleurl.so \
out/Debug/obj.target/chrome/libcommon_constants.a \
out/Debug/obj.target/content/libcontent.so \
out/Debug/obj.target/skia/libskia.so \
out/Debug/obj.target/third_party/libxml/libxml2.a \
out/Debug/obj.target/third_party/mt19937ar/libmt19937ar.a \
out/Debug/obj.target/third_party/sqlite/libsqlite3.a \
out/Debug/obj.target/third_party/zlib/libminizip.a \
out/Debug/obj.target/webkit/support/libuser_agent.so \
out/Debug/obj.target/chrome/app/policy/libpolicy.a \
out/Debug/obj.target/chrome/app/policy/libcloud_policy_proto_compile.a \
out/Debug/obj.target/chrome/app/policy/libcloud_policy_backend_header_compile.a \
out/Debug/obj.target/chrome/common/extensions/api/libapi.a \
out/Debug/obj.target/tools/json_schema_compiler/libapi_gen_util.a \
out/Debug/obj.target/ipc/libipc.so \
out/Debug/obj.target/third_party/re2/libre2.a \
out/Debug/obj.target/webkit/support/libglue.so \
out/Debug/obj.target/remoting/libremoting_client_plugin.a \
out/Debug/obj.target/remoting/libremoting_base.a \
out/Debug/obj.target/ui/libui.so \
out/Debug/obj.target/third_party/libvpx/libvpx.a \
out/Debug/obj.target/third_party/libvpx/libvpx_asm_offsets.a \
out/Debug/obj.target/third_party/opus/libopus.a \
out/Debug/obj.target/third_party/speex/libspeex.a \
out/Debug/obj.target/media/libmedia.so \
out/Debug/obj.target/media/libshared_memory_support.so \
out/Debug/obj.target/media/libyuv_convert.a \
out/Debug/obj.target/media/libyuv_convert_simd_x86.a \
out/Debug/obj.target/remoting/libremoting_jingle_glue.a \
out/Debug/obj.target/jingle/libjingle_glue.a \
out/Debug/obj.target/third_party/libjingle/libjingle.a \
out/Debug/obj.target/third_party/jsoncpp/libjsoncpp.a \
out/Debug/obj.target/third_party/libjingle/libjingle_p2p.a \
out/Debug/obj.target/jingle/libnotifier.a \
out/Debug/obj.target/remoting/proto/libchromotocol_proto_lib.a \
out/Debug/obj.target/remoting/libremoting_client.a \
out/Debug/obj.target/remoting/libremoting_protocol.a \
out/Debug/obj.target/ppapi/libppapi_cpp_objects.a \
out/Debug/obj.target/chrome/browser/component/libnavigation_interception.a \
out/Debug/obj.target/chrome/browser/performance_monitor/libperformance_monitor.a \
out/Debug/obj.target/chrome/libbrowser_extensions.a \
out/Debug/obj.target/sync/protocol/libsync_proto.a \
out/Debug/obj.target/chrome/libcontacts_proto.a \
out/Debug/obj.target/net/libhttp_server.a \
out/Debug/obj.target/third_party/leveldatabase/libleveldatabase.a \
out/Debug/obj.target/chrome/libin_memory_url_index_cache_proto.a \
out/Debug/obj.target/device/libdevice_bluetooth.a \
out/Debug/obj.target/sync/libsync_notifier.a \
out/Debug/obj.target/third_party/cacheinvalidation/libcacheinvalidation.a \
out/Debug/obj.target/third_party/cacheinvalidation/libcacheinvalidation_proto_cpp.a \
out/Debug/obj.target/sync/libsync.a \
out/Debug/obj.target/google_apis/libgoogle_apis.a \
out/Debug/obj.target/sql/libsql.so \
out/Debug/obj.target/third_party/libusb/libusb.a \
out/Debug/obj.target/webkit/support/libwebkit_base.so \
out/Debug/obj.target/webkit/support/libwebkit_storage.so \
out/Debug/obj.target/dbus/libdbus.a \
out/Debug/obj.target/third_party/undoview/libundoview.a \
out/Debug/obj.target/chrome/libbrowser_ui.a \
out/Debug/obj.target/chrome/libcert_logger_proto.a \
out/Debug/obj.target/chrome/libsafe_browsing_proto.a \
out/Debug/obj.target/chrome/libsafe_browsing_report_proto.a \
out/Debug/obj.target/chrome/libfeedback_proto.a \
out/Debug/obj.target/chrome/libvariations_seed_proto.a \
out/Debug/obj.target/net/libnet_with_v8.so \
out/Debug/obj.target/ppapi/libppapi_proxy.so \
out/Debug/obj.target/sync/libsyncapi_service.a \
out/Debug/obj.target/third_party/cld/libcld.a \
out/Debug/obj.target/third_party/hunspell/libhunspell.a \
out/Debug/obj.target/third_party/libphonenumber/libphonenumber.a \
out/Debug/obj.target/third_party/libphonenumber/libphonenumber_without_metadata.a \
out/Debug/obj.target/ui/compositor/libcompositor.so \
out/Debug/obj.target/ui/surface/libsurface.so \
out/Debug/obj.target/ui/web_dialogs/libweb_dialogs.so \
out/Debug/obj.target/v8/tools/gyp/libv8.so \
out/Debug/obj.target/native_client/src/trusted/service_runtime/libsel.a \
out/Debug/obj.target/native_client/src/trusted/service_runtime/libenv_cleanser.a \
out/Debug/obj.target/native_client/src/trusted/service_runtime/libnacl_error_code.a \
out/Debug/obj.target/native_client/src/shared/gio/libgio.a \
out/Debug/obj.target/native_client/src/shared/platform/libplatform.a \
out/Debug/obj.target/native_client/src/shared/srpc/libnonnacl_srpc.a \
out/Debug/obj.target/native_client/src/trusted/debug_stub/libdebug_stub.a \
out/Debug/obj.target/native_client/src/trusted/desc/libnrd_xfer.a \
out/Debug/obj.target/native_client/src/trusted/desc/libdesc_wrapper.a \
out/Debug/obj.target/native_client/src/shared/imc/libimc.a \
out/Debug/obj.target/native_client/src/trusted/nacl_base/libnacl_base.a \
out/Debug/obj.target/native_client/src/trusted/fault_injection/libnacl_fault_inject.a \
out/Debug/obj.target/native_client/src/trusted/gio/libgio_wrapped_desc.a \
out/Debug/obj.target/native_client/src/trusted/interval_multiset/libnacl_interval.a \
out/Debug/obj.target/native_client/src/trusted/perf_counter/libnacl_perf_counter.a \
out/Debug/obj.target/native_client/src/trusted/platform_qualify/libplatform_qual_lib.a \
out/Debug/obj.target/native_client/src/trusted/validator/x86/libncval_base_x86_64.a \
out/Debug/obj.target/native_client/src/trusted/manifest_name_service_proxy/libmanifest_proxy.a \
out/Debug/obj.target/native_client/src/trusted/threading/libthread_interface.a \
out/Debug/obj.target/native_client/src/trusted/simple_service/libsimple_service.a \
out/Debug/obj.target/native_client/src/trusted/validator/libvalidators.a \
out/Debug/obj.target/native_client/src/trusted/service_runtime/arch/x86/libservice_runtime_x86_common.a \
out/Debug/obj.target/native_client/src/trusted/validator/x86/64/libncvalidate_x86_64.a \
out/Debug/obj.target/native_client/src/trusted/validator/x86/ncval_reg_sfi/libncval_reg_sfi_x86_64.a \
out/Debug/obj.target/native_client/src/trusted/validator_x86/libnccopy_x86_64.a \
out/Debug/obj.target/native_client/src/trusted/validator/x86/decoder/libnc_decoder_x86_64.a \
out/Debug/obj.target/native_client/src/trusted/validator/x86/decoder/libnc_opcode_modeling_x86_64.a \
out/Debug/obj.target/native_client/src/trusted/service_runtime/arch/x86_64/libservice_runtime_x86_64.a \
out/Debug/obj.target/chrome/libsync_file_system_proto.a \
out/Debug/obj.target/chrome/libmtp_file_entry_proto.a \
out/Debug/obj.target/chrome/libmtp_storage_info_proto.a \
out/Debug/obj.target/ppapi/libppapi_host.so \
out/Debug/obj.target/ppapi/libppapi_shared.so \
out/Debug/obj.target/third_party/WebKit/Source/WebKit/chromium/libwebkit.so \
out/Debug/obj.target/webkit/support/libwebkit_gpu.a \
out/Debug/obj.target/gpu/command_buffer/libgles2_utils.so \
out/Debug/obj.target/gpu/libgles2_c_lib.so \
out/Debug/obj.target/gpu/libgles2_implementation.so \
out/Debug/obj.target/third_party/angle/src/libtranslator_glsl.so \
out/Debug/obj.target/ui/gl/libgl_wrapper.so \
out/Debug/obj.target/webkit/support/libwebkit_media.a \
out/Debug/obj.target/chrome/libnacl.a \
out/Debug/obj.target/third_party/smhasher/libmurmurhash3.a \
out/Debug/obj.target/sandbox/libsuid_sandbox_client.a \
out/Debug/obj.target/sandbox/linux/seccomp-legacy/libseccomp_sandbox.a \
out/Debug/obj.target/sandbox/libseccomp_bpf.a

LINK_LIBS_SYS = \
-lX11 \
-lXcursor \
-lXrandr \
-lXrender \
-lXss \
-lXext \
-lrt \
-ldl \
-lnss3 \
-lnssutil3 \
-lsmime3 \
-lplds4 \
-lplc4 \
-lnspr4 \
-lpthread \
-lgmodule-2.0 \
-lgtk-x11-2.0 \
-lgdk-x11-2.0 \
-latk-1.0 \
-lgio-2.0 \
-lpangoft2-1.0 \
-lpangocairo-1.0 \
-lgdk_pixbuf-2.0 \
-lcairo \
-lpango-1.0 \
-lfreetype \
-lfontconfig \
-lgobject-2.0 \
-lgthread-2.0 \
-lglib-2.0 \
-lXcomposite \
-lbz2 \
-lcups \
-lgssapi_krb5 \
-lkrb5 \
-lk5crypto \
-lcom_err \
-lgnutls \
-lgcrypt \
-lz \
-lcrypt \
-lm \
-lasound \
-lexpat \
-ldbus-1 \
-lgconf-2 \
-lXi \
-ludev

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
	g++ -fexceptions -std=c++0x -DBERKELIUM_CPP_IMPL -Isrc/include -I../berkelium-cpp/include -I../chromium/src/third_party/skia/include/config -I../chromium/src -D_DEBUG -DDEBUG -g -Wall -pthread -fno-exceptions -fno-strict-aliasing -Wno-unused-parameter -Wno-missing-field-initializers -fvisibility=hidden -pipe -fPIC -O0 -g -fno-rtti -fno-threadsafe-statics -fvisibility-inlines-hidden -MMD -MF $@.d.raw -c -o $@ $<

run/berkelium: $(SOURCES:%.cpp=out/o/%.o)
	test -d run || mkdir run
	for i in chrome.pak chrome_100_percent.pak resources.pak locales nacl_helper_bootstrap nacl_helper nacl_irt_x86_64.nexe; do test -e run/$$i || ln -s ../out/Debug/$$i run ; done
	g++ -L../build/out $(LINK_ARGS) -o run/berkelium -Wl,--start-group $(LINK_OBJS) $^ $(LINK_LIBS_OWN) -Wl,--end-group $(LINK_LIBS_SYS) -lberkelium-cpp -lboost_filesystem -lboost_system

.PHONY: run
run: run/berkelium
	./berkelium
