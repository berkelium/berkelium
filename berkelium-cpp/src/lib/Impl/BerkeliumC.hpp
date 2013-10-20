// Copyright (c) 2013 The Berkelium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

using Berkelium::impl::bk_error;

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
	}

	virtual ~BkLogDelegateMapper() {
		free(delegate);
	}

	virtual void log(Berkelium::RuntimeRef runtime, Berkelium::LogSource source, Berkelium::LogType type,
			 const std::string& clazz, const std::string& name, const std::string& message) {
		delegate->log(env, delegate, NULL, Host, StdErr,
			      malloc(clazz), malloc(name), malloc(message));
	}
};

class BkHostDelegateMapper : public Berkelium::HostDelegate
{
private:
	BK_Env* env;
	BK_HostDelegate delegate;
public:
	BkHostDelegateMapper(BK_Env* env, BK_HostDelegate delegate)
		: Berkelium::HostDelegate(),
		env(env),
		delegate(delegate) {
	}

	virtual ~BkHostDelegateMapper() {
		free(delegate);
	}

	virtual void onCrashed(Berkelium::InstanceRef instance) {
		bk_error("BkHostDelegateMapper::onCrashed not full implemented");
		delegate->onCrashed(env, delegate, NULL);
	}

	virtual void onClosed(Berkelium::InstanceRef instance) {
		bk_error("BkHostDelegateMapper::onClosed not full implemented");
		delegate->onClosed(env, delegate, NULL);
	}

	virtual void onPing(Berkelium::InstanceRef instance) {
		bk_error("BkHostDelegateMapper::onPing not full implemented");
		delegate->onPing(env, delegate, NULL);
	}
};

class BkTabDelegateMapper : public Berkelium::TabDelegate
{
private:
	BK_Env* env;
	BK_TabDelegate delegate;
public:
	BkTabDelegateMapper(BK_Env* env, BK_TabDelegate delegate)
		: Berkelium::TabDelegate(),
		env(env),
		delegate(delegate) {
	}

	virtual ~BkTabDelegateMapper() {
		free(delegate);
	}

	virtual void onClosed(Berkelium::TabRef tab) {
		bk_error("BkTabDelegateMapper::onClosed not implemented");
	}

	virtual void onTitleChanged(Berkelium::TabRef tab, const std::string& title) {
		bk_error("BkTabDelegateMapper::onTitleChanged not implemented");
	}

	virtual void onPaint(Berkelium::TabRef tab) {
		bk_error("BkTabDelegateMapper::onPaint not implemented");
	}

	virtual void onPaintDone(Berkelium::TabRef tab, Berkelium::RectRef rect) {
		bk_error("BkTabDelegateMapper::onPaintDone not implemented");
	}

	virtual void onReady(Berkelium::TabRef tab) {
		bk_error("BkTabDelegateMapper::onReady not implemented");
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

inline Berkelium::LogDelegateRef mapInLogDelegateRef(BK_Env* env, bk_ext_obj extId)
{
	BERKELIUM_C_TRACE_STATIC();

	BK_LogDelegate log = (BK_LogDelegate)env->mapIn(LogDelegate, extId, env->data);
	if(log == NULL) {
		bk_error("error: '%s' returned NULL!", __FUNCTION__);
		return Berkelium::LogDelegateRef();
	}

	BERKELIUM_C_TRACE_RETURN(log);

	return Berkelium::LogDelegateRef(new BkLogDelegateMapper(env, log));
}

inline Berkelium::HostDelegateRef mapInHostDelegateRef(BK_Env* env, bk_ext_obj extId)
{
	BERKELIUM_C_TRACE_STATIC();

	BK_HostDelegate host = (BK_HostDelegate)env->mapIn(HostDelegate, extId, env->data);
	if(host == NULL) {
		bk_error("error: '%s' returned NULL!", __FUNCTION__);
		return Berkelium::HostDelegateRef();
	}

	BERKELIUM_C_TRACE_RETURN(host);

	return Berkelium::HostDelegateRef(new BkHostDelegateMapper(env, host));
}

inline Berkelium::TabDelegateRef mapInTabDelegateRef(BK_Env* env, bk_ext_obj extId)
{
	BERKELIUM_C_TRACE_STATIC();

	BK_TabDelegate tab = (BK_TabDelegate)env->mapIn(TabDelegate, extId, env->data);
	if(tab == NULL) {
		bk_error("error: '%s' returned NULL!", __FUNCTION__);
		return Berkelium::TabDelegateRef();
	}

	BERKELIUM_C_TRACE_RETURN(tab);

	return Berkelium::TabDelegateRef(new BkTabDelegateMapper(env, tab));
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
	bk_error("Null Pointer Exception: Class '%s' - Argument '%s'", clazz, arg);
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
