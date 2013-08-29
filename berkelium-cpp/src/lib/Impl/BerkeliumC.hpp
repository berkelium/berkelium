// Copyright (c) 2013 The Berkelium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

/*
class JavaLogDelegate : public Berkelium::LogDelegate
{
private:
	BK_Env* env;
public:
	JavaLogDelegate()
		: Berkelium::LogDelegate() {
	}

	virtual ~JavaLogDelegate() {
	}

	virtual void log(Berkelium::RuntimeRef runtime, Berkelium::LogSource source, Berkelium::LogType type, const std::string& clazz, const std::string& name, const std::string& message) {
		fprintf(stderr, "log: %s %s %s", clazz.c_str(), name.c_str(), message.c_str());
	}
};
*/


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


inline Berkelium::LogDelegateRef mapInLogDelegateRef(BK_Env* env, BK_LogDelegate log)
{
	BERKELIUM_C_TRACE_STATIC();
	return Berkelium::LogDelegateRef();
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

bk_ext_obj mapNew(BK_Env_Enum type, bk_bk_obj id, void* data)
{
	BERKELIUM_C_TRACE_NOENV();
	return (bk_ext_obj)id;
}

BK_Env create()
{
	BK_Env ret;

	ret.mapIn = &mapIn;
	ret.mapOut = &mapOut;
	ret.mapNew = &mapNew;

	return ret;
}

BK_Env env = create();

} // namespace
