// Copyright (c) 2013 The Berkelium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

using Berkelium::impl::bk_error;

inline char* newString(const std::string& msg) {
	int n = msg.length() + 1;
	char* ret = (char*)malloc(n);
	memcpy(ret, msg.c_str(), n);
	return ret;
}

inline Berkelium::RectRef mapInRectRef(BK_Env* env, BK_Rect& rect)
{
	// TODO
	return Berkelium::RectRef();
}

inline bk_ext_obj mapOutRectRef(BK_Env* env, RectRef rect)
{
	// TODO
	return NULL;
}

#if defined(BERKELIUM_TRACE_C_ENTER) || defined(BERKELIUM_TRACE_C_LEAVE)
class BerkeliumCTracer {
	BK_Env* env;
	const char* fnk;
public:
	BerkeliumCTracer(BK_Env* env, const char* fnk) :
		env(env),
		fnk(fnk) {
#ifdef BERKELIUM_TRACE_C_ENTER
		bk_error(stderr,"ENTER %s (env:%p)", fnk, env);
#endif
	}

	~BerkeliumCTracer() {
#ifdef BERKELIUM_TRACE_C_LEAVE
		bk_error(stderr,"LEAVE %s (env:%p)", fnk, env);
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
#define BERKELIUM_C_TRACE_RETURN(ret) bk_error("RETURN %s = %p", __FUNCTION__, ret);
#else
#define BERKELIUM_C_TRACE_RETURN(ret)
#endif

namespace simpleBerkeliumEnv {

bk_bk_obj mapIn(BK_Env_Enum type, bk_ext_obj id, BK_Env* env)
{
	BERKELIUM_C_TRACE_NOENV();
	return (bk_bk_obj)id;
}

bk_ext_obj mapOut(BK_Env_Enum type, bk_bk_obj id, BK_Env* env)
{
	BERKELIUM_C_TRACE_NOENV();
	return (bk_ext_obj)id;
}

bk_ext_obj mapNew(BK_Env_Enum type, bk_bk_obj id, bk_ext_obj extId,  BK_Env* env)
{
	BERKELIUM_C_TRACE_NOENV();
	return (bk_ext_obj)id;
}

void NPE(bk_string clazz, bk_string arg)
{
	bk_error("Null Pointer Exception: Class '%s' - Argument '%s'", clazz, arg);
}

void free(bk_ext_obj extId, BK_Env* env)
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
	ret.cloned = false;

	return ret;
}

BK_Env env = create();

} // namespace

BK_Env* BK_Env_clone(BK_Env* env)
{
	BK_Env* ret = (BK_Env*)malloc(sizeof(BK_Env));

	ret->mapIn = env->mapIn;
	ret->mapOut = env->mapOut;
	ret->mapNew = env->mapNew;
	ret->free= env->free;
	ret->NPE = env->NPE;
	ret->cloned = true;

	return ret;
}
