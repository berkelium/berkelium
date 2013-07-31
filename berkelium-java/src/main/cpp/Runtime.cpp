#include <Berkelium/API/Runtime.hpp>

#include "berkelium-jni.hpp"

JNIEXPORT jobject JNICALL Java_org_berkelium_impl_RuntimeImpl_forSystemInstalled(JNIEnv* env, jobject)
{
	return bk_new_JNI_Runtime(env);
}
