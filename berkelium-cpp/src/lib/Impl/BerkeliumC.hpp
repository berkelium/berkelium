// Copyright (c) 2013 The Berkelium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

inline char* malloc(const std::string& msg) {
	int n = msg.length() + 1;
	char* ret = (char*)malloc(n);
	memcpy(ret, msg.c_str(), n);
	return ret;
}

class BkLogDelegateMapper : public Berkelium::LogDelegate
{
private:
	BK_Env* env;
	BK_LogDelegate delegate;
public:
	BkLogDelegateMapper(BK_Env* env, BK_LogDelegate delegate)
		: Berkelium::LogDelegate(),
		env(env),
		delegate(delegate) {
		//fprintf(stderr, "new BkLogDelegateMapper\n");
	}

	virtual ~BkLogDelegateMapper() {
		//fprintf(stderr, "delete BkLogDelegateMapper\n");
		free(delegate);
	}

	virtual void log(Berkelium::RuntimeRef runtime, Berkelium::LogSource source, Berkelium::LogType type,
			 const std::string& clazz, const std::string& name, const std::string& message) {
		delegate->log(env, delegate, NULL, Host, StdErr,
			      malloc(clazz), malloc(name), malloc(message));
	}
};

#if defined(BERKELIUM_TRACE_C_ENTER) || defined(BERKELIUM_TRACE_C_LEAVE)
class BerkeliumCTracer {
	BK_Env* env;
	const char* fnk;
public:
	BerkeliumCTracer(BK_Env* env, const char* fnk) :
		env(env),
		fnk(fnk) {
#ifdef BERKELIUM_TRACE_C_ENTER
		fprintf(stderr,"ENTER %s (env:%p)\n", fnk, env);
#endif
	}

	~BerkeliumCTracer() {
#ifdef BERKELIUM_TRACE_C_LEAVE
		fprintf(stderr,"LEAVE %s (env:%p)\n", fnk, env);
#endif
	}
};

#define BERKELIUM_C_TRACE() BerkeliumCTracer tracer(env, __FUNCTION__);
#define BERKELIUM_C_TRACE_STATIC() BerkeliumCTracer tracer(env, __FUNCTION__);
#define BERKELIUM_C_TRACE_NOENV() BerkeliumCTracer tracer(NULL, __FUNCTION__);
#else
#define BERKELIUM_C_TRACE()
#define BERKELIUM_C_TRACE_STATIC()
#define BERKELIUM_C_TRACE_NOENV()
#endif

#ifdef BERKELIUM_TRACE_C_RETURN
#define BERKELIUM_C_TRACE_RETURN(ret) fprintf(stderr, "RETURN %s = %p\n", __FUNCTION__, ret);
#else
#define BERKELIUM_C_TRACE_RETURN(ret)
#endif

inline BK_TabDelegate mapOutTabDelegateRef(BK_Env* env, Berkelium::TabDelegateRef bk)
{
	BERKELIUM_C_TRACE_STATIC();
	return NULL;
}

inline Berkelium::LogDelegateRef mapInLogDelegateRef(BK_Env* env, bk_ext_obj extId)
{
	BERKELIUM_C_TRACE_STATIC();

	BK_LogDelegate log = (BK_LogDelegate)env->mapIn(LogDelegate, extId, env->data);
	/*
	if(log == NULL) {
		fprintf(stderr, "error: '%s' returned NULL!\n", __FUNCTION__);
		return Berkelium::LogDelegateRef();
	}
	*/

	BERKELIUM_C_TRACE_RETURN(log);

	return Berkelium::LogDelegateRef(new BkLogDelegateMapper(env, log));
}

inline Berkelium::HostDelegateRef mapInHostDelegateRef(BK_Env* env, BK_HostDelegate host)
{
	BERKELIUM_C_TRACE_STATIC();
	return Berkelium::HostDelegateRef();
}

inline Berkelium::TabDelegateRef mapInTabDelegateRef(BK_Env* env, BK_TabDelegate tab)
{
	BERKELIUM_C_TRACE_STATIC();
	return Berkelium::TabDelegateRef();
}

namespace simpleBerkeliumEnv {

bk_bk_obj mapIn(BK_Env_Enum type, bk_ext_obj id, void* data)
{
	BERKELIUM_C_TRACE_NOENV();
	return (bk_bk_obj)id;
}

bk_ext_obj mapOut(BK_Env_Enum type, bk_bk_obj id, void* data)
{
	BERKELIUM_C_TRACE_NOENV();
	return (bk_ext_obj)id;
}

bk_ext_obj mapNew(BK_Env_Enum type, bk_bk_obj id, bk_ext_obj extId,  void* data)
{
	BERKELIUM_C_TRACE_NOENV();
	return (bk_ext_obj)id;
}

void NPE(bk_string clazz, bk_string arg)
{
	fprintf(stderr, "Null Pointer Exception: Class '%s' - Argument '%s'", clazz, arg);
}

void free(bk_ext_obj extId, void* data)
{
}

BK_Env create()
{
	BK_Env ret;

	ret.mapIn = &mapIn;
	ret.mapOut = &mapOut;
	ret.mapNew = &mapNew;
	ret.free= &free;
	ret.NPE = &NPE;

	return ret;
}

BK_Env env = create();

} // namespace
