// Copyright (c) 2013 The Berkelium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BERKELIUM_JNI_HPP__
#define BERKELIUM_JNI_HPP__

#include "berkelium.h"

#include <stdlib.h>
#include <string.h>

#include <sstream>

extern const char* BK_Java_Class_Names[];

jclass BK_Java_Classes[BK_Env_Enum_MAX];
jmethodID BK_Java_Class_ctors[BK_Env_Enum_MAX];

#define JavaID_IllegalArgumentException "java/lang/IllegalArgumentException"
jclass IllegalArgumentException_class;

#define JavaID_BerkeliumJavaImpl "org/berkelium/impl/BerkeliumJavaImpl"
jclass BerkeliumJavaImpl_class;

#define JavaID_BerkeliumJavaImpl_mapIn "(ILorg/berkelium/impl/BerkeliumObjectImpl;)J"
jmethodID BerkeliumJavaImpl_mapIn_Java;
#define JavaID_BerkeliumJavaImpl_mapOut "(IJ)Lorg/berkelium/impl/BerkeliumObjectImpl;"
jmethodID BerkeliumJavaImpl_mapOut_Java;
#define JavaID_BerkeliumJavaImpl_mapNew "(IJLorg/berkelium/impl/BerkeliumObjectImpl;)V"
jmethodID BerkeliumJavaImpl_mapNew_Java;
#define JavaID_BerkeliumJavaImpl_free "(J)V"
jmethodID BerkeliumJavaImpl_free_Java;

#define JavaID_LogSource "org/berkelium/api/LogSource"
jclass LogSource_class;
#define JavaID_LogSource_ordinal "()I"
jmethodID LogSource_ordinal_Java;

#define JavaID_LogType "org/berkelium/api/LogType"
jclass LogType_class;
#define JavaID_LogType_ordinal "()I"
jmethodID LogType_ordinal_Java;

#define JavaID_LogDelegate "org/berkelium/api/LogDelegate"
jclass LogDelegate_class;
#define JavaID_LogDelegate_log "(Lorg/berkelium/api/Runtime;Lorg/berkelium/api/LogSource;Lorg/berkelium/api/LogType;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)V"
jmethodID LogDelegate_log_Java;

#define BERKELIUM_FIND_CLASS(n) \
	c = env->FindClass(JavaID_ ## n);					\
	if(c == NULL) {								\
		return JNI_ERR;							\
	}									\
	n ## _class = (jclass)env->NewGlobalRef(c);

#define BERKELIUM_GET_STATIC(c, n)						\
	m = env->GetStaticMethodID(c ## _class, #n, JavaID_ ## c ## _ ## n);	\
	if(m == NULL) {								\
		return JNI_ERR;							\
	}									\
	c ## _ ## n ## _Java = m;

#define BERKELIUM_GET_METHODID(c, n)						\
	m = env->GetMethodID(c ## _class, #n, JavaID_ ## c ## _ ## n);		\
	if(m == NULL) {								\
		return JNI_ERR;							\
	}									\
	c ## _ ## n ## _Java = m;

JavaVM* berkelium_jvm = NULL;

JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM* jvm, void* reserved)
{
	jclass c;
	jmethodID m;
	JNIEnv* env;

	if(jvm->GetEnv((void**)&env, JNI_VERSION_1_1)) {
		return JNI_ERR;
	}

	BERKELIUM_FIND_CLASS(IllegalArgumentException);

	BERKELIUM_FIND_CLASS(BerkeliumJavaImpl);
	BERKELIUM_GET_STATIC(BerkeliumJavaImpl, mapIn);
	BERKELIUM_GET_STATIC(BerkeliumJavaImpl, mapOut);
	BERKELIUM_GET_STATIC(BerkeliumJavaImpl, mapNew);
	BERKELIUM_GET_STATIC(BerkeliumJavaImpl, free);

	BERKELIUM_FIND_CLASS(LogSource);
	BERKELIUM_GET_METHODID(LogSource, ordinal);

	BERKELIUM_FIND_CLASS(LogType);
	BERKELIUM_GET_METHODID(LogType, ordinal);

	BERKELIUM_FIND_CLASS(LogDelegate);
	BERKELIUM_GET_METHODID(LogDelegate, log);

	for(int i = 0; i < BK_Env_Enum_MAX; i++) {
		c = env->FindClass(BK_Java_Class_Names[i]);
		if(c == NULL) {
			return JNI_ERR;
		}
		c = (jclass)env->NewGlobalRef(c);
		BK_Java_Classes[i] = c;
		jmethodID m = env->GetMethodID(c, "<init>", "()V");
		if(m == NULL) {
			return JNI_ERR;
		}
		BK_Java_Class_ctors[i] = m;
	}

	berkelium_jvm = jvm;

	return JNI_VERSION_1_1;
}

inline JNIEnv* getBerkeliumJavaVMForCallback()
{
	JNIEnv* env = NULL;
	if(berkelium_jvm->GetEnv((void**)&env, JNI_VERSION_1_1)) {
		fprintf(stderr, "berkelium java vm error!\n");
	}
	return env;
}

#define BK_TO_JOBJECT(X) (jobject)(X)
#define BK_TO_JBOOLEAN(X) ((X) ? JNI_TRUE : JNI_FALSE)

inline jstring BK_TO_JSTRING(JNIEnv* env, char* str)
{
	if(str == NULL) {
		return NULL;
	}
	jstring ret = (jstring)env->NewGlobalRef(env->NewStringUTF(str));
	free(str);
	return ret;
}

inline char* JSTRING_TO_BK(JNIEnv* env, jstring str)
{
	if(str == NULL) {
		return NULL;
	}
	jboolean iscopy = false;
	const char* tmp = env->GetStringUTFChars(str, &iscopy);
	jint len = env->GetStringUTFLength(str) + 1;
	char* ret = (char*)malloc(len);
	memcpy(ret, tmp, len);
	env->ReleaseStringUTFChars(str, tmp);
	return ret;
}

void Berkelium_Java_LogDelegate(BK_Env* bkenv, BK_LogDelegate self, BK_Runtime runtime, BK_LogSource source, BK_LogType type, bk_string clazz, bk_string name, bk_string message)
{
	JNIEnv* env = getBerkeliumJavaVMForCallback();

	//fprintf(stderr, "log: %s %s %s\n", clazz, name, message);
	if(env != NULL) {
		env->CallVoidMethod((jobject)self->self, LogDelegate_log_Java, NULL, NULL, NULL, BK_TO_JSTRING(env, clazz), BK_TO_JSTRING(env, name), BK_TO_JSTRING(env, message));
	}
	//fprintf(stderr, "log done!\n");
}

bk_bk_obj Berkelium_Java_MapIn(BK_Env_Enum type, bk_ext_obj bkJavaId, void* data)
{
	//fprintf(stderr, "Berkelium_Java_MapIn\n");
	JNIEnv* env = (JNIEnv*)data;
	bk_bk_obj ret((bk_bk_obj)env->CallStaticLongMethod(BerkeliumJavaImpl_class, BerkeliumJavaImpl_mapIn_Java, type, bkJavaId));

	if(!ret) {
		if(type == LogDelegate) {
			BK_LogDelegate ret((BK_LogDelegate)malloc(sizeof(_BK_LogDelegate)));
			ret->log = &Berkelium_Java_LogDelegate;
			ret->self = env->NewGlobalRef((jobject)bkJavaId);
			//fprintf(stderr, "Berkelium_Java_MapIn: LogDelegate local:%p global:%p env:%p\n", bkJavaId, ret->self, env);
			return ret;
		}
		fprintf(stderr, "MapIn: unknown type %d %p\n", type, ret);
	}

	return ret;
}

bk_ext_obj Berkelium_Java_MapOut(BK_Env_Enum type, bk_bk_obj bkNativeId, void* data)
{
	//fprintf(stderr, "Berkelium_Java_MapOut\n");
	JNIEnv* env = (JNIEnv*)data;
	return env->CallStaticObjectMethod(BerkeliumJavaImpl_class, BerkeliumJavaImpl_mapOut_Java, type, bkNativeId);
}

bk_ext_obj Berkelium_Java_MapNew(BK_Env_Enum type, bk_bk_obj bkNativeId, bk_ext_obj bkJavaId, void* data)
{
	//fprintf(stderr, "Berkelium_Java_MapNew\n");
	JNIEnv* env = (JNIEnv*)data;
	if(type < 0 || type >= BK_Env_Enum_MAX) {
		return NULL;
	}

	if(bkJavaId == NULL) {
		bkJavaId = env->NewObject(BK_Java_Classes[type], BK_Java_Class_ctors[type]);
	}

	env->CallStaticVoidMethod(BerkeliumJavaImpl_class, BerkeliumJavaImpl_mapNew_Java, type, bkNativeId, bkJavaId);
	return bkJavaId;
}

void Berkelium_Java_NPE(bk_string clazz, bk_string arg)
{
	JNIEnv* env(getBerkeliumJavaVMForCallback());
	if(env) {
		std::stringstream ss;
		ss << "Null Pointer Exception: Class '" << clazz << "' - Argument '" << arg << "'";
		env->ThrowNew(IllegalArgumentException_class, ss.str().c_str());
	} else {
		fprintf(stderr, "Null Pointer Exception: Class '%s' - Argument '%s'", clazz, arg);
	}
}

void Berkelium_Java_Free(bk_ext_obj extId, void* data)
{
	JNIEnv* env = (JNIEnv*)data;
	env->CallStaticVoidMethod(BerkeliumJavaImpl_class, BerkeliumJavaImpl_free_Java, extId);
}

inline void setupBkEnv(BK_Env& bkenv, JNIEnv* env)
{
	bkenv.mapIn = Berkelium_Java_MapIn;
	bkenv.mapOut = Berkelium_Java_MapOut;
	bkenv.mapNew = Berkelium_Java_MapNew;
	bkenv.free = Berkelium_Java_Free;
	bkenv.NPE = Berkelium_Java_NPE;
	bkenv.data = env;
}

inline BK_LogSource LogSource_TO_BK(JNIEnv* env, jobject instance)
{
	return (BK_LogSource)env->CallIntMethod(instance, LogSource_ordinal_Java);
}

inline BK_LogType LogType_TO_BK(JNIEnv* env, jobject instance)
{
	return (BK_LogType)env->CallIntMethod(instance, LogType_ordinal_Java);
}

#endif // BERKELIUM_JNI_HPP__
