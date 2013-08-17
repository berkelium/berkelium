// Copyright (c) 2013 The Berkelium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

// =========================================
// WARNING:
//   THIS FILE IS AUTOMATICALLY GENERATED!
//   !! ANY CHANGES WILL BE OVERWRITTEN !!
//
// See berkelium/berkelium-api/update.sh
// =========================================

#include <Berkelium/API/HostVersion.hpp>
#include <Berkelium/API/BerkeliumFactory.hpp>
#include <Berkelium/API/Runtime.hpp>
#include <Berkelium/API/HostExecutable.hpp>
#include <Berkelium/API/Profile.hpp>
#include <Berkelium/API/Logger.hpp>
#include <Berkelium/API/LogDelegate.hpp>
#include <Berkelium/API/HostDelegate.hpp>
#include <Berkelium/API/Instance.hpp>
#include <Berkelium/API/Window.hpp>
#include <Berkelium/API/WindowDelegate.hpp>
#include <Berkelium/API/Tab.hpp>
#include <Berkelium/API/TabDelegate.hpp>

#include "berkelium.h"

#include <stdlib.h>
#include <string.h>

//#include <map>

// =========================================
// C / C++ converter functions
// =========================================

//typedef std::pair<bk_ext_obj, BK_Env_Enum> bk_type_pair;
//typedef std::map<bk_ext_obj, BK_Env_Enum> bk_type_map;
//bk_type_map bk_types;

inline char* newString(const std::string& str)
{
	int len = str.length() + 1;
	char* ret = (char*)malloc(len);
	memcpy(ret, str.c_str(), len);
	return ret;
}

inline Berkelium::RectRef mapInRectRef(BK_Env* env, BK_Rect& rect)
{
	// TODO
	return Berkelium::RectRef();
}


inline Berkelium::HostVersionRef mapInHostVersionRef(BK_Env* env, bk_ext_obj extId)
{
	BK_Env_Enum type(HostVersion);

	/*bk_type_map::iterator it(bk_types.find(extId));
	if(it == bk_types.end()) {
		fprintf(stderr, "mapIn HostVersion ext:%p not found\n", extId);
		return Berkelium::HostVersionRef();
	} else if(it->second != type) {
		env->mapInError(type, it->second, extId, env->data);
		return Berkelium::HostVersionRef();
	}
	fprintf(stderr, "mapIn %p found\n", extId);*/

	void* tmp = env->mapIn(type, extId, env->data);
	//fprintf(stderr, "mapIn %p\n", tmp);

	Berkelium::HostVersionRef* ret((Berkelium::HostVersionRef*)tmp);

	return Berkelium::HostVersionRef(*ret);
}

inline bk_ext_obj mapOutHostVersionRef(BK_Env* env, Berkelium::HostVersionRef bk)
{
	BK_Env_Enum type(HostVersion);

	bk_ext_obj ret = env->mapOut(type, bk.get(), env->data);

	if(ret == NULL) {
		void* tmp = new Berkelium::HostVersionRef(bk);
		ret = env->mapNew(type, bk.get(), tmp, env->data);
		//bk_types.insert(bk_type_pair(ret, type));
		//fprintf(stderr, "mapNew %p = %d (%p)\n", ret, type, tmp);
	}

	//fprintf(stderr, "mapOut HostVersion bk:%p ext:%p\n", bk.get(), ret);

	return ret;
}

inline Berkelium::BerkeliumFactoryRef mapInBerkeliumFactoryRef(BK_Env* env, bk_ext_obj extId)
{
	BK_Env_Enum type(BerkeliumFactory);

	/*bk_type_map::iterator it(bk_types.find(extId));
	if(it == bk_types.end()) {
		fprintf(stderr, "mapIn BerkeliumFactory ext:%p not found\n", extId);
		return Berkelium::BerkeliumFactoryRef();
	} else if(it->second != type) {
		env->mapInError(type, it->second, extId, env->data);
		return Berkelium::BerkeliumFactoryRef();
	}
	fprintf(stderr, "mapIn %p found\n", extId);*/

	void* tmp = env->mapIn(type, extId, env->data);
	//fprintf(stderr, "mapIn %p\n", tmp);

	Berkelium::BerkeliumFactoryRef* ret((Berkelium::BerkeliumFactoryRef*)tmp);

	return Berkelium::BerkeliumFactoryRef(*ret);
}

inline bk_ext_obj mapOutBerkeliumFactoryRef(BK_Env* env, Berkelium::BerkeliumFactoryRef bk)
{
	BK_Env_Enum type(BerkeliumFactory);

	bk_ext_obj ret = env->mapOut(type, bk.get(), env->data);

	if(ret == NULL) {
		void* tmp = new Berkelium::BerkeliumFactoryRef(bk);
		ret = env->mapNew(type, bk.get(), tmp, env->data);
		//bk_types.insert(bk_type_pair(ret, type));
		//fprintf(stderr, "mapNew %p = %d (%p)\n", ret, type, tmp);
	}

	//fprintf(stderr, "mapOut BerkeliumFactory bk:%p ext:%p\n", bk.get(), ret);

	return ret;
}

inline Berkelium::RuntimeRef mapInRuntimeRef(BK_Env* env, bk_ext_obj extId)
{
	BK_Env_Enum type(Runtime);

	/*bk_type_map::iterator it(bk_types.find(extId));
	if(it == bk_types.end()) {
		fprintf(stderr, "mapIn Runtime ext:%p not found\n", extId);
		return Berkelium::RuntimeRef();
	} else if(it->second != type) {
		env->mapInError(type, it->second, extId, env->data);
		return Berkelium::RuntimeRef();
	}
	fprintf(stderr, "mapIn %p found\n", extId);*/

	void* tmp = env->mapIn(type, extId, env->data);
	//fprintf(stderr, "mapIn %p\n", tmp);

	Berkelium::RuntimeRef* ret((Berkelium::RuntimeRef*)tmp);

	return Berkelium::RuntimeRef(*ret);
}

inline bk_ext_obj mapOutRuntimeRef(BK_Env* env, Berkelium::RuntimeRef bk)
{
	BK_Env_Enum type(Runtime);

	bk_ext_obj ret = env->mapOut(type, bk.get(), env->data);

	if(ret == NULL) {
		void* tmp = new Berkelium::RuntimeRef(bk);
		ret = env->mapNew(type, bk.get(), tmp, env->data);
		//bk_types.insert(bk_type_pair(ret, type));
		//fprintf(stderr, "mapNew %p = %d (%p)\n", ret, type, tmp);
	}

	//fprintf(stderr, "mapOut Runtime bk:%p ext:%p\n", bk.get(), ret);

	return ret;
}

inline Berkelium::HostExecutableRef mapInHostExecutableRef(BK_Env* env, bk_ext_obj extId)
{
	BK_Env_Enum type(HostExecutable);

	/*bk_type_map::iterator it(bk_types.find(extId));
	if(it == bk_types.end()) {
		fprintf(stderr, "mapIn HostExecutable ext:%p not found\n", extId);
		return Berkelium::HostExecutableRef();
	} else if(it->second != type) {
		env->mapInError(type, it->second, extId, env->data);
		return Berkelium::HostExecutableRef();
	}
	fprintf(stderr, "mapIn %p found\n", extId);*/

	void* tmp = env->mapIn(type, extId, env->data);
	//fprintf(stderr, "mapIn %p\n", tmp);

	Berkelium::HostExecutableRef* ret((Berkelium::HostExecutableRef*)tmp);

	return Berkelium::HostExecutableRef(*ret);
}

inline bk_ext_obj mapOutHostExecutableRef(BK_Env* env, Berkelium::HostExecutableRef bk)
{
	BK_Env_Enum type(HostExecutable);

	bk_ext_obj ret = env->mapOut(type, bk.get(), env->data);

	if(ret == NULL) {
		void* tmp = new Berkelium::HostExecutableRef(bk);
		ret = env->mapNew(type, bk.get(), tmp, env->data);
		//bk_types.insert(bk_type_pair(ret, type));
		//fprintf(stderr, "mapNew %p = %d (%p)\n", ret, type, tmp);
	}

	//fprintf(stderr, "mapOut HostExecutable bk:%p ext:%p\n", bk.get(), ret);

	return ret;
}

inline Berkelium::ProfileRef mapInProfileRef(BK_Env* env, bk_ext_obj extId)
{
	BK_Env_Enum type(Profile);

	/*bk_type_map::iterator it(bk_types.find(extId));
	if(it == bk_types.end()) {
		fprintf(stderr, "mapIn Profile ext:%p not found\n", extId);
		return Berkelium::ProfileRef();
	} else if(it->second != type) {
		env->mapInError(type, it->second, extId, env->data);
		return Berkelium::ProfileRef();
	}
	fprintf(stderr, "mapIn %p found\n", extId);*/

	void* tmp = env->mapIn(type, extId, env->data);
	//fprintf(stderr, "mapIn %p\n", tmp);

	Berkelium::ProfileRef* ret((Berkelium::ProfileRef*)tmp);

	return Berkelium::ProfileRef(*ret);
}

inline bk_ext_obj mapOutProfileRef(BK_Env* env, Berkelium::ProfileRef bk)
{
	BK_Env_Enum type(Profile);

	bk_ext_obj ret = env->mapOut(type, bk.get(), env->data);

	if(ret == NULL) {
		void* tmp = new Berkelium::ProfileRef(bk);
		ret = env->mapNew(type, bk.get(), tmp, env->data);
		//bk_types.insert(bk_type_pair(ret, type));
		//fprintf(stderr, "mapNew %p = %d (%p)\n", ret, type, tmp);
	}

	//fprintf(stderr, "mapOut Profile bk:%p ext:%p\n", bk.get(), ret);

	return ret;
}

inline Berkelium::LoggerRef mapInLoggerRef(BK_Env* env, bk_ext_obj extId)
{
	BK_Env_Enum type(Logger);

	/*bk_type_map::iterator it(bk_types.find(extId));
	if(it == bk_types.end()) {
		fprintf(stderr, "mapIn Logger ext:%p not found\n", extId);
		return Berkelium::LoggerRef();
	} else if(it->second != type) {
		env->mapInError(type, it->second, extId, env->data);
		return Berkelium::LoggerRef();
	}
	fprintf(stderr, "mapIn %p found\n", extId);*/

	void* tmp = env->mapIn(type, extId, env->data);
	//fprintf(stderr, "mapIn %p\n", tmp);

	Berkelium::LoggerRef* ret((Berkelium::LoggerRef*)tmp);

	return Berkelium::LoggerRef(*ret);
}

inline bk_ext_obj mapOutLoggerRef(BK_Env* env, Berkelium::LoggerRef bk)
{
	BK_Env_Enum type(Logger);

	bk_ext_obj ret = env->mapOut(type, bk.get(), env->data);

	if(ret == NULL) {
		void* tmp = new Berkelium::LoggerRef(bk);
		ret = env->mapNew(type, bk.get(), tmp, env->data);
		//bk_types.insert(bk_type_pair(ret, type));
		//fprintf(stderr, "mapNew %p = %d (%p)\n", ret, type, tmp);
	}

	//fprintf(stderr, "mapOut Logger bk:%p ext:%p\n", bk.get(), ret);

	return ret;
}

inline Berkelium::LogDelegateRef mapInLogDelegateRef(BK_Env* env, bk_ext_obj extId)
{
	BK_Env_Enum type(LogDelegate);

	/*bk_type_map::iterator it(bk_types.find(extId));
	if(it == bk_types.end()) {
		fprintf(stderr, "mapIn LogDelegate ext:%p not found\n", extId);
		return Berkelium::LogDelegateRef();
	} else if(it->second != type) {
		env->mapInError(type, it->second, extId, env->data);
		return Berkelium::LogDelegateRef();
	}
	fprintf(stderr, "mapIn %p found\n", extId);*/

	void* tmp = env->mapIn(type, extId, env->data);
	//fprintf(stderr, "mapIn %p\n", tmp);

	Berkelium::LogDelegateRef* ret((Berkelium::LogDelegateRef*)tmp);

	return Berkelium::LogDelegateRef(*ret);
}

inline bk_ext_obj mapOutLogDelegateRef(BK_Env* env, Berkelium::LogDelegateRef bk)
{
	BK_Env_Enum type(LogDelegate);

	bk_ext_obj ret = env->mapOut(type, bk.get(), env->data);

	if(ret == NULL) {
		void* tmp = new Berkelium::LogDelegateRef(bk);
		ret = env->mapNew(type, bk.get(), tmp, env->data);
		//bk_types.insert(bk_type_pair(ret, type));
		//fprintf(stderr, "mapNew %p = %d (%p)\n", ret, type, tmp);
	}

	//fprintf(stderr, "mapOut LogDelegate bk:%p ext:%p\n", bk.get(), ret);

	return ret;
}

inline Berkelium::HostDelegateRef mapInHostDelegateRef(BK_Env* env, bk_ext_obj extId)
{
	BK_Env_Enum type(HostDelegate);

	/*bk_type_map::iterator it(bk_types.find(extId));
	if(it == bk_types.end()) {
		fprintf(stderr, "mapIn HostDelegate ext:%p not found\n", extId);
		return Berkelium::HostDelegateRef();
	} else if(it->second != type) {
		env->mapInError(type, it->second, extId, env->data);
		return Berkelium::HostDelegateRef();
	}
	fprintf(stderr, "mapIn %p found\n", extId);*/

	void* tmp = env->mapIn(type, extId, env->data);
	//fprintf(stderr, "mapIn %p\n", tmp);

	Berkelium::HostDelegateRef* ret((Berkelium::HostDelegateRef*)tmp);

	return Berkelium::HostDelegateRef(*ret);
}

inline bk_ext_obj mapOutHostDelegateRef(BK_Env* env, Berkelium::HostDelegateRef bk)
{
	BK_Env_Enum type(HostDelegate);

	bk_ext_obj ret = env->mapOut(type, bk.get(), env->data);

	if(ret == NULL) {
		void* tmp = new Berkelium::HostDelegateRef(bk);
		ret = env->mapNew(type, bk.get(), tmp, env->data);
		//bk_types.insert(bk_type_pair(ret, type));
		//fprintf(stderr, "mapNew %p = %d (%p)\n", ret, type, tmp);
	}

	//fprintf(stderr, "mapOut HostDelegate bk:%p ext:%p\n", bk.get(), ret);

	return ret;
}

inline Berkelium::InstanceRef mapInInstanceRef(BK_Env* env, bk_ext_obj extId)
{
	BK_Env_Enum type(Instance);

	/*bk_type_map::iterator it(bk_types.find(extId));
	if(it == bk_types.end()) {
		fprintf(stderr, "mapIn Instance ext:%p not found\n", extId);
		return Berkelium::InstanceRef();
	} else if(it->second != type) {
		env->mapInError(type, it->second, extId, env->data);
		return Berkelium::InstanceRef();
	}
	fprintf(stderr, "mapIn %p found\n", extId);*/

	void* tmp = env->mapIn(type, extId, env->data);
	//fprintf(stderr, "mapIn %p\n", tmp);

	Berkelium::InstanceRef* ret((Berkelium::InstanceRef*)tmp);

	return Berkelium::InstanceRef(*ret);
}

inline bk_ext_obj mapOutInstanceRef(BK_Env* env, Berkelium::InstanceRef bk)
{
	BK_Env_Enum type(Instance);

	bk_ext_obj ret = env->mapOut(type, bk.get(), env->data);

	if(ret == NULL) {
		void* tmp = new Berkelium::InstanceRef(bk);
		ret = env->mapNew(type, bk.get(), tmp, env->data);
		//bk_types.insert(bk_type_pair(ret, type));
		//fprintf(stderr, "mapNew %p = %d (%p)\n", ret, type, tmp);
	}

	//fprintf(stderr, "mapOut Instance bk:%p ext:%p\n", bk.get(), ret);

	return ret;
}

inline Berkelium::WindowRef mapInWindowRef(BK_Env* env, bk_ext_obj extId)
{
	BK_Env_Enum type(Window);

	/*bk_type_map::iterator it(bk_types.find(extId));
	if(it == bk_types.end()) {
		fprintf(stderr, "mapIn Window ext:%p not found\n", extId);
		return Berkelium::WindowRef();
	} else if(it->second != type) {
		env->mapInError(type, it->second, extId, env->data);
		return Berkelium::WindowRef();
	}
	fprintf(stderr, "mapIn %p found\n", extId);*/

	void* tmp = env->mapIn(type, extId, env->data);
	//fprintf(stderr, "mapIn %p\n", tmp);

	Berkelium::WindowRef* ret((Berkelium::WindowRef*)tmp);

	return Berkelium::WindowRef(*ret);
}

inline bk_ext_obj mapOutWindowRef(BK_Env* env, Berkelium::WindowRef bk)
{
	BK_Env_Enum type(Window);

	bk_ext_obj ret = env->mapOut(type, bk.get(), env->data);

	if(ret == NULL) {
		void* tmp = new Berkelium::WindowRef(bk);
		ret = env->mapNew(type, bk.get(), tmp, env->data);
		//bk_types.insert(bk_type_pair(ret, type));
		//fprintf(stderr, "mapNew %p = %d (%p)\n", ret, type, tmp);
	}

	//fprintf(stderr, "mapOut Window bk:%p ext:%p\n", bk.get(), ret);

	return ret;
}

inline Berkelium::WindowDelegateRef mapInWindowDelegateRef(BK_Env* env, bk_ext_obj extId)
{
	BK_Env_Enum type(WindowDelegate);

	/*bk_type_map::iterator it(bk_types.find(extId));
	if(it == bk_types.end()) {
		fprintf(stderr, "mapIn WindowDelegate ext:%p not found\n", extId);
		return Berkelium::WindowDelegateRef();
	} else if(it->second != type) {
		env->mapInError(type, it->second, extId, env->data);
		return Berkelium::WindowDelegateRef();
	}
	fprintf(stderr, "mapIn %p found\n", extId);*/

	void* tmp = env->mapIn(type, extId, env->data);
	//fprintf(stderr, "mapIn %p\n", tmp);

	Berkelium::WindowDelegateRef* ret((Berkelium::WindowDelegateRef*)tmp);

	return Berkelium::WindowDelegateRef(*ret);
}

inline bk_ext_obj mapOutWindowDelegateRef(BK_Env* env, Berkelium::WindowDelegateRef bk)
{
	BK_Env_Enum type(WindowDelegate);

	bk_ext_obj ret = env->mapOut(type, bk.get(), env->data);

	if(ret == NULL) {
		void* tmp = new Berkelium::WindowDelegateRef(bk);
		ret = env->mapNew(type, bk.get(), tmp, env->data);
		//bk_types.insert(bk_type_pair(ret, type));
		//fprintf(stderr, "mapNew %p = %d (%p)\n", ret, type, tmp);
	}

	//fprintf(stderr, "mapOut WindowDelegate bk:%p ext:%p\n", bk.get(), ret);

	return ret;
}

inline Berkelium::TabRef mapInTabRef(BK_Env* env, bk_ext_obj extId)
{
	BK_Env_Enum type(Tab);

	/*bk_type_map::iterator it(bk_types.find(extId));
	if(it == bk_types.end()) {
		fprintf(stderr, "mapIn Tab ext:%p not found\n", extId);
		return Berkelium::TabRef();
	} else if(it->second != type) {
		env->mapInError(type, it->second, extId, env->data);
		return Berkelium::TabRef();
	}
	fprintf(stderr, "mapIn %p found\n", extId);*/

	void* tmp = env->mapIn(type, extId, env->data);
	//fprintf(stderr, "mapIn %p\n", tmp);

	Berkelium::TabRef* ret((Berkelium::TabRef*)tmp);

	return Berkelium::TabRef(*ret);
}

inline bk_ext_obj mapOutTabRef(BK_Env* env, Berkelium::TabRef bk)
{
	BK_Env_Enum type(Tab);

	bk_ext_obj ret = env->mapOut(type, bk.get(), env->data);

	if(ret == NULL) {
		void* tmp = new Berkelium::TabRef(bk);
		ret = env->mapNew(type, bk.get(), tmp, env->data);
		//bk_types.insert(bk_type_pair(ret, type));
		//fprintf(stderr, "mapNew %p = %d (%p)\n", ret, type, tmp);
	}

	//fprintf(stderr, "mapOut Tab bk:%p ext:%p\n", bk.get(), ret);

	return ret;
}

inline Berkelium::TabDelegateRef mapInTabDelegateRef(BK_Env* env, bk_ext_obj extId)
{
	BK_Env_Enum type(TabDelegate);

	/*bk_type_map::iterator it(bk_types.find(extId));
	if(it == bk_types.end()) {
		fprintf(stderr, "mapIn TabDelegate ext:%p not found\n", extId);
		return Berkelium::TabDelegateRef();
	} else if(it->second != type) {
		env->mapInError(type, it->second, extId, env->data);
		return Berkelium::TabDelegateRef();
	}
	fprintf(stderr, "mapIn %p found\n", extId);*/

	void* tmp = env->mapIn(type, extId, env->data);
	//fprintf(stderr, "mapIn %p\n", tmp);

	Berkelium::TabDelegateRef* ret((Berkelium::TabDelegateRef*)tmp);

	return Berkelium::TabDelegateRef(*ret);
}

inline bk_ext_obj mapOutTabDelegateRef(BK_Env* env, Berkelium::TabDelegateRef bk)
{
	BK_Env_Enum type(TabDelegate);

	bk_ext_obj ret = env->mapOut(type, bk.get(), env->data);

	if(ret == NULL) {
		void* tmp = new Berkelium::TabDelegateRef(bk);
		ret = env->mapNew(type, bk.get(), tmp, env->data);
		//bk_types.insert(bk_type_pair(ret, type));
		//fprintf(stderr, "mapNew %p = %d (%p)\n", ret, type, tmp);
	}

	//fprintf(stderr, "mapOut TabDelegate bk:%p ext:%p\n", bk.get(), ret);

	return ret;
}

inline BK_WindowList* mapOutWindowList(BK_Env* env, Berkelium::WindowListRef list)
{
	BK_WindowList* ret = (BK_WindowList*)malloc(sizeof(BK_WindowList));

	ret->size = list->size();
	ret->entrys = (BK_Window*)malloc(sizeof(BK_Window) * ret->size);

	int i = 0;
	for(Berkelium::WindowList::iterator it(list->begin()); it != list->end(); i++, it++) {
		ret->entrys[i] = mapOutWindowRef(env, *it);
	}

	return ret;
}

inline BK_TabList* mapOutTabList(BK_Env* env, Berkelium::TabListRef list)
{
	BK_TabList* ret = (BK_TabList*)malloc(sizeof(BK_TabList));

	ret->size = list->size();
	ret->entrys = (BK_Tab*)malloc(sizeof(BK_Tab) * ret->size);

	int i = 0;
	for(Berkelium::TabList::iterator it(list->begin()); it != list->end(); i++, it++) {
		ret->entrys[i] = mapOutTabRef(env, *it);
	}

	return ret;
}

// =========================================
// BK_Env_Enum to String
// =========================================

const char* BK_Env_Enum_To_String_Or_Null(BK_Env_Enum type)
{
	switch(type) {
		case HostVersion:
			return "HostVersion";

		case BerkeliumFactory:
			return "BerkeliumFactory";

		case Runtime:
			return "Runtime";

		case HostExecutable:
			return "HostExecutable";

		case Profile:
			return "Profile";

		case Logger:
			return "Logger";

		case LogDelegate:
			return "LogDelegate";

		case HostDelegate:
			return "HostDelegate";

		case Instance:
			return "Instance";

		case Window:
			return "Window";

		case WindowDelegate:
			return "WindowDelegate";

		case Tab:
			return "Tab";

		case TabDelegate:
			return "TabDelegate";

		default:
			return NULL;
	}
}

const char* BK_Env_Enum_To_String_Or_Err(BK_Env_Enum type)
{
	const char* ret = BK_Env_Enum_To_String_Or_Null(type);
	if(ret == NULL) {
		return "Error";
	}
	return ret;
}

// =========================================
// interface HostVersion
// =========================================

extern "C" BK_Runtime BK_HostVersion_getRuntime(BK_Env* env, BK_HostVersion self)
{

	Berkelium::HostVersionRef _this(mapInHostVersionRef(env, self));

	if(!_this) {
		return NULL;
	}

	return mapOutRuntimeRef(env, _this->getRuntime());
}

extern "C" bk_string BK_HostVersion_getVersionString(BK_Env* env, BK_HostVersion self)
{

	Berkelium::HostVersionRef _this(mapInHostVersionRef(env, self));

	if(!_this) {
		return NULL;
	}

	return newString(_this->getVersionString());
}

extern "C" bk_int32 BK_HostVersion_getMajor(BK_Env* env, BK_HostVersion self)
{

	Berkelium::HostVersionRef _this(mapInHostVersionRef(env, self));

	if(!_this) {
		return 0;
	}

	return _this->getMajor();
}

extern "C" bk_int32 BK_HostVersion_getMinor(BK_Env* env, BK_HostVersion self)
{

	Berkelium::HostVersionRef _this(mapInHostVersionRef(env, self));

	if(!_this) {
		return 0;
	}

	return _this->getMinor();
}

extern "C" bk_int32 BK_HostVersion_getBuild(BK_Env* env, BK_HostVersion self)
{

	Berkelium::HostVersionRef _this(mapInHostVersionRef(env, self));

	if(!_this) {
		return 0;
	}

	return _this->getBuild();
}

extern "C" bk_int32 BK_HostVersion_getPatch(BK_Env* env, BK_HostVersion self)
{

	Berkelium::HostVersionRef _this(mapInHostVersionRef(env, self));

	if(!_this) {
		return 0;
	}

	return _this->getPatch();
}

extern "C" bk_bool BK_HostVersion_isMinVersion(BK_Env* env, BK_HostVersion self, bk_string version)
{

	Berkelium::HostVersionRef _this(mapInHostVersionRef(env, self));

	if(!_this) {
		return false;
	}

	return _this->isMinVersion(version);
}

// =========================================
// class BerkeliumFactory
// =========================================

extern "C" BK_Runtime BK_BerkeliumFactory_getDefaultRuntime(BK_Env* env)
{
	return mapOutRuntimeRef(env, Berkelium::BerkeliumFactory::getDefaultRuntime());
}

extern "C" BK_Runtime BK_BerkeliumFactory_createRuntime(BK_Env* env)
{
	return mapOutRuntimeRef(env, Berkelium::BerkeliumFactory::createRuntime());
}

// =========================================
// interface Runtime
// =========================================

extern "C" void BK_Runtime_setDefaultExecutable(BK_Env* env, BK_Runtime self, bk_string pathTo)
{

	Berkelium::RuntimeRef _this(mapInRuntimeRef(env, self));

	if(!_this) {
		return;
	}

	_this->setDefaultExecutable(pathTo);
}

extern "C" BK_HostExecutable BK_Runtime_forExecutable(BK_Env* env, BK_Runtime self, bk_string pathTo)
{

	Berkelium::RuntimeRef _this(mapInRuntimeRef(env, self));

	if(!_this) {
		return NULL;
	}

	return mapOutHostExecutableRef(env, _this->forExecutable(pathTo));
}

extern "C" BK_HostExecutable BK_Runtime_forSystemInstalled(BK_Env* env, BK_Runtime self)
{

	Berkelium::RuntimeRef _this(mapInRuntimeRef(env, self));

	if(!_this) {
		return NULL;
	}

	return mapOutHostExecutableRef(env, _this->forSystemInstalled());
}

extern "C" BK_Profile BK_Runtime_forProfile(BK_Env* env, BK_Runtime self, bk_string application)
{

	Berkelium::RuntimeRef _this(mapInRuntimeRef(env, self));

	if(!_this) {
		return NULL;
	}

	return mapOutProfileRef(env, _this->forProfile(application));
}

extern "C" BK_Profile BK_Runtime_getChromeProfile(BK_Env* env, BK_Runtime self)
{

	Berkelium::RuntimeRef _this(mapInRuntimeRef(env, self));

	if(!_this) {
		return NULL;
	}

	return mapOutProfileRef(env, _this->getChromeProfile());
}

extern "C" BK_Profile BK_Runtime_getChromiumProfile(BK_Env* env, BK_Runtime self)
{

	Berkelium::RuntimeRef _this(mapInRuntimeRef(env, self));

	if(!_this) {
		return NULL;
	}

	return mapOutProfileRef(env, _this->getChromiumProfile());
}

extern "C" BK_Profile BK_Runtime_forProfilePath(BK_Env* env, BK_Runtime self, bk_string path)
{

	Berkelium::RuntimeRef _this(mapInRuntimeRef(env, self));

	if(!_this) {
		return NULL;
	}

	return mapOutProfileRef(env, _this->forProfilePath(path));
}

extern "C" BK_Profile BK_Runtime_createTemporaryProfile(BK_Env* env, BK_Runtime self)
{

	Berkelium::RuntimeRef _this(mapInRuntimeRef(env, self));

	if(!_this) {
		return NULL;
	}

	return mapOutProfileRef(env, _this->createTemporaryProfile());
}

extern "C" BK_HostVersion BK_Runtime_forVersionS(BK_Env* env, BK_Runtime self, bk_string version)
{

	Berkelium::RuntimeRef _this(mapInRuntimeRef(env, self));

	if(!_this) {
		return NULL;
	}

	return mapOutHostVersionRef(env, _this->forVersion(version));
}

extern "C" BK_HostVersion BK_Runtime_forVersion4I(BK_Env* env, BK_Runtime self, bk_int32 vMajor, bk_int32 vMinor, bk_int32 vBuild, bk_int32 vPatch)
{

	Berkelium::RuntimeRef _this(mapInRuntimeRef(env, self));

	if(!_this) {
		return NULL;
	}

	return mapOutHostVersionRef(env, _this->forVersion(vMajor, vMinor, vBuild, vPatch));
}

extern "C" BK_Instance BK_Runtime_open(BK_Env* env, BK_Runtime self, BK_HostExecutable executable, BK_Profile profile)
{

	Berkelium::RuntimeRef _this(mapInRuntimeRef(env, self));

	if(!_this) {
		return NULL;
	}

	return mapOutInstanceRef(env, _this->open(mapInHostExecutableRef(env, executable), mapInProfileRef(env, profile)));
}

extern "C" BK_Logger BK_Runtime_getLogger(BK_Env* env, BK_Runtime self, bk_string clazz, bk_string name)
{

	Berkelium::RuntimeRef _this(mapInRuntimeRef(env, self));

	if(!_this) {
		return NULL;
	}

	return mapOutLoggerRef(env, _this->getLogger(clazz, name));
}

extern "C" void BK_Runtime_addLogDelegate(BK_Env* env, BK_Runtime self, BK_LogDelegate delegate)
{

	Berkelium::RuntimeRef _this(mapInRuntimeRef(env, self));

	if(!_this) {
		return;
	}

	_this->addLogDelegate(mapInLogDelegateRef(env, delegate));
}

extern "C" void BK_Runtime_removeLogDelegate(BK_Env* env, BK_Runtime self, BK_LogDelegate delegate)
{

	Berkelium::RuntimeRef _this(mapInRuntimeRef(env, self));

	if(!_this) {
		return;
	}

	_this->removeLogDelegate(mapInLogDelegateRef(env, delegate));
}

extern "C" void BK_Runtime_log(BK_Env* env, BK_Runtime self, BK_LogSource source, BK_LogType type, bk_string clazz, bk_string name, bk_string message)
{

	Berkelium::RuntimeRef _this(mapInRuntimeRef(env, self));

	if(!_this) {
		return;
	}

	_this->log((Berkelium::LogSource)source, (Berkelium::LogType)type, clazz, name, message);
}

// =========================================
// interface HostExecutable
// =========================================

extern "C" BK_Runtime BK_HostExecutable_getRuntime(BK_Env* env, BK_HostExecutable self)
{

	Berkelium::HostExecutableRef _this(mapInHostExecutableRef(env, self));

	if(!_this) {
		return NULL;
	}

	return mapOutRuntimeRef(env, _this->getRuntime());
}

extern "C" bk_string BK_HostExecutable_getPath(BK_Env* env, BK_HostExecutable self)
{

	Berkelium::HostExecutableRef _this(mapInHostExecutableRef(env, self));

	if(!_this) {
		return NULL;
	}

	return newString(_this->getPath());
}

extern "C" BK_HostVersion BK_HostExecutable_getVersion(BK_Env* env, BK_HostExecutable self)
{

	Berkelium::HostExecutableRef _this(mapInHostExecutableRef(env, self));

	if(!_this) {
		return NULL;
	}

	return mapOutHostVersionRef(env, _this->getVersion());
}

// =========================================
// interface Profile
// =========================================

extern "C" BK_Runtime BK_Profile_getRuntime(BK_Env* env, BK_Profile self)
{

	Berkelium::ProfileRef _this(mapInProfileRef(env, self));

	if(!_this) {
		return NULL;
	}

	return mapOutRuntimeRef(env, _this->getRuntime());
}

extern "C" bk_bool BK_Profile_isInUse(BK_Env* env, BK_Profile self)
{

	Berkelium::ProfileRef _this(mapInProfileRef(env, self));

	if(!_this) {
		return false;
	}

	return _this->isInUse();
}

extern "C" bk_bool BK_Profile_isFound(BK_Env* env, BK_Profile self)
{

	Berkelium::ProfileRef _this(mapInProfileRef(env, self));

	if(!_this) {
		return false;
	}

	return _this->isFound();
}

extern "C" bk_bool BK_Profile_isSameVersion(BK_Env* env, BK_Profile self)
{

	Berkelium::ProfileRef _this(mapInProfileRef(env, self));

	if(!_this) {
		return false;
	}

	return _this->isSameVersion();
}

extern "C" bk_bool BK_Profile_isTooNew(BK_Env* env, BK_Profile self)
{

	Berkelium::ProfileRef _this(mapInProfileRef(env, self));

	if(!_this) {
		return false;
	}

	return _this->isTooNew();
}

extern "C" bk_string BK_Profile_getApplicationName(BK_Env* env, BK_Profile self)
{

	Berkelium::ProfileRef _this(mapInProfileRef(env, self));

	if(!_this) {
		return NULL;
	}

	return newString(_this->getApplicationName());
}

extern "C" bk_string BK_Profile_getProfilePath(BK_Env* env, BK_Profile self)
{

	Berkelium::ProfileRef _this(mapInProfileRef(env, self));

	if(!_this) {
		return NULL;
	}

	return newString(_this->getProfilePath());
}

extern "C" void BK_Profile_setLocked(BK_Env* env, BK_Profile self, bk_bool locked)
{

	Berkelium::ProfileRef _this(mapInProfileRef(env, self));

	if(!_this) {
		return;
	}

	_this->setLocked(locked);
}

extern "C" bk_bool BK_Profile_isLocked(BK_Env* env, BK_Profile self)
{

	Berkelium::ProfileRef _this(mapInProfileRef(env, self));

	if(!_this) {
		return false;
	}

	return _this->isLocked();
}

extern "C" void BK_Logger_debug(BK_Env* env, BK_Logger self, bk_string message)
{

	Berkelium::LoggerRef _this(mapInLoggerRef(env, self));

	if(!_this) {
		return;
	}

	_this->debug(message);
}

extern "C" void BK_Logger_info(BK_Env* env, BK_Logger self, bk_string message)
{

	Berkelium::LoggerRef _this(mapInLoggerRef(env, self));

	if(!_this) {
		return;
	}

	_this->info(message);
}

extern "C" void BK_Logger_warn(BK_Env* env, BK_Logger self, bk_string message)
{

	Berkelium::LoggerRef _this(mapInLoggerRef(env, self));

	if(!_this) {
		return;
	}

	_this->warn(message);
}

extern "C" void BK_Logger_error(BK_Env* env, BK_Logger self, bk_string message)
{

	Berkelium::LoggerRef _this(mapInLoggerRef(env, self));

	if(!_this) {
		return;
	}

	_this->error(message);
}

extern "C" void BK_LogDelegate_log(BK_Env* env, BK_LogDelegate self, BK_Runtime runtime, BK_LogSource source, BK_LogType type, bk_string clazz, bk_string name, bk_string message)
{

	Berkelium::LogDelegateRef _this(mapInLogDelegateRef(env, self));

	if(!_this) {
		return;
	}

	_this->log(mapInRuntimeRef(env, runtime), (Berkelium::LogSource)source, (Berkelium::LogType)type, clazz, name, message);
}

extern "C" void BK_HostDelegate_onCrashed(BK_Env* env, BK_HostDelegate self, BK_Instance instance)
{

	Berkelium::HostDelegateRef _this(mapInHostDelegateRef(env, self));

	if(!_this) {
		return;
	}

	_this->onCrashed(mapInInstanceRef(env, instance));
}

extern "C" void BK_HostDelegate_onClosed(BK_Env* env, BK_HostDelegate self, BK_Instance instance)
{

	Berkelium::HostDelegateRef _this(mapInHostDelegateRef(env, self));

	if(!_this) {
		return;
	}

	_this->onClosed(mapInInstanceRef(env, instance));
}

// =========================================
// interface Instance
// =========================================

extern "C" BK_Runtime BK_Instance_getRuntime(BK_Env* env, BK_Instance self)
{

	Berkelium::InstanceRef _this(mapInInstanceRef(env, self));

	if(!_this) {
		return NULL;
	}

	return mapOutRuntimeRef(env, _this->getRuntime());
}

extern "C" void BK_Instance_internalUpdate(BK_Env* env, BK_Instance self)
{

	Berkelium::InstanceRef _this(mapInInstanceRef(env, self));

	if(!_this) {
		return;
	}

	_this->internalUpdate();
}

extern "C" void BK_Instance_close(BK_Env* env, BK_Instance self)
{

	Berkelium::InstanceRef _this(mapInInstanceRef(env, self));

	if(!_this) {
		return;
	}

	_this->close();
}

extern "C" BK_Profile BK_Instance_getProfile(BK_Env* env, BK_Instance self)
{

	Berkelium::InstanceRef _this(mapInInstanceRef(env, self));

	if(!_this) {
		return NULL;
	}

	return mapOutProfileRef(env, _this->getProfile());
}

extern "C" BK_HostExecutable BK_Instance_getExecutable(BK_Env* env, BK_Instance self)
{

	Berkelium::InstanceRef _this(mapInInstanceRef(env, self));

	if(!_this) {
		return NULL;
	}

	return mapOutHostExecutableRef(env, _this->getExecutable());
}

extern "C" void BK_Instance_addHostDelegate(BK_Env* env, BK_Instance self, BK_HostDelegate delegate)
{

	Berkelium::InstanceRef _this(mapInInstanceRef(env, self));

	if(!_this) {
		return;
	}

	_this->addHostDelegate(mapInHostDelegateRef(env, delegate));
}

extern "C" void BK_Instance_removeHostDelegate(BK_Env* env, BK_Instance self, BK_HostDelegate delegate)
{

	Berkelium::InstanceRef _this(mapInInstanceRef(env, self));

	if(!_this) {
		return;
	}

	_this->removeHostDelegate(mapInHostDelegateRef(env, delegate));
}

extern "C" bk_int32 BK_Instance_getWindowCount(BK_Env* env, BK_Instance self)
{

	Berkelium::InstanceRef _this(mapInInstanceRef(env, self));

	if(!_this) {
		return 0;
	}

	return _this->getWindowCount();
}

extern "C" BK_WindowList* BK_Instance_getWindowList(BK_Env* env, BK_Instance self)
{

	Berkelium::InstanceRef _this(mapInInstanceRef(env, self));

	if(!_this) {
		return NULL;
	}

	return mapOutWindowList(env, _this->getWindowList());
}

extern "C" BK_Window BK_Instance_createWindow(BK_Env* env, BK_Instance self, bk_bool incognito)
{

	Berkelium::InstanceRef _this(mapInInstanceRef(env, self));

	if(!_this) {
		return NULL;
	}

	return mapOutWindowRef(env, _this->createWindow(incognito));
}

// =========================================
// interface Window
// =========================================

extern "C" BK_Runtime BK_Window_getRuntime(BK_Env* env, BK_Window self)
{

	Berkelium::WindowRef _this(mapInWindowRef(env, self));

	if(!_this) {
		return NULL;
	}

	return mapOutRuntimeRef(env, _this->getRuntime());
}

extern "C" void BK_Window_internalUpdate(BK_Env* env, BK_Window self)
{

	Berkelium::WindowRef _this(mapInWindowRef(env, self));

	if(!_this) {
		return;
	}

	_this->internalUpdate();
}

extern "C" bk_int32 BK_Window_getTabCount(BK_Env* env, BK_Window self)
{

	Berkelium::WindowRef _this(mapInWindowRef(env, self));

	if(!_this) {
		return 0;
	}

	return _this->getTabCount();
}

extern "C" BK_TabList* BK_Window_getTabList(BK_Env* env, BK_Window self)
{

	Berkelium::WindowRef _this(mapInWindowRef(env, self));

	if(!_this) {
		return NULL;
	}

	return mapOutTabList(env, _this->getTabList());
}

extern "C" BK_Tab BK_Window_createTab(BK_Env* env, BK_Window self)
{

	Berkelium::WindowRef _this(mapInWindowRef(env, self));

	if(!_this) {
		return NULL;
	}

	return mapOutTabRef(env, _this->createTab());
}

extern "C" BK_Instance BK_Window_getInstance(BK_Env* env, BK_Window self)
{

	Berkelium::WindowRef _this(mapInWindowRef(env, self));

	if(!_this) {
		return NULL;
	}

	return mapOutInstanceRef(env, _this->getInstance());
}

extern "C" void BK_Window_moveTo(BK_Env* env, BK_Window self, BK_Tab tab, bk_int32 index)
{

	Berkelium::WindowRef _this(mapInWindowRef(env, self));

	if(!_this) {
		return;
	}

	_this->moveTo(mapInTabRef(env, tab), index);
}

extern "C" bk_bool BK_Window_isIncognito(BK_Env* env, BK_Window self)
{

	Berkelium::WindowRef _this(mapInWindowRef(env, self));

	if(!_this) {
		return false;
	}

	return _this->isIncognito();
}

// =========================================
// interface Tab
// =========================================

extern "C" BK_Runtime BK_Tab_getRuntime(BK_Env* env, BK_Tab self)
{

	Berkelium::TabRef _this(mapInTabRef(env, self));

	if(!_this) {
		return NULL;
	}

	return mapOutRuntimeRef(env, _this->getRuntime());
}

extern "C" void BK_Tab_internalUpdate(BK_Env* env, BK_Tab self)
{

	Berkelium::TabRef _this(mapInTabRef(env, self));

	if(!_this) {
		return;
	}

	_this->internalUpdate();
}

extern "C" void BK_Tab_close(BK_Env* env, BK_Tab self)
{

	Berkelium::TabRef _this(mapInTabRef(env, self));

	if(!_this) {
		return;
	}

	_this->close();
}

extern "C" void BK_Tab_sync(BK_Env* env, BK_Tab self)
{

	Berkelium::TabRef _this(mapInTabRef(env, self));

	if(!_this) {
		return;
	}

	_this->sync();
}

extern "C" BK_TabDelegate BK_Tab_getTabDelegate(BK_Env* env, BK_Tab self)
{

	Berkelium::TabRef _this(mapInTabRef(env, self));

	if(!_this) {
		return NULL;
	}

	return mapOutTabDelegateRef(env, _this->getTabDelegate());
}

extern "C" void BK_Tab_addTabDelegate(BK_Env* env, BK_Tab self, BK_TabDelegate delegate)
{

	Berkelium::TabRef _this(mapInTabRef(env, self));

	if(!_this) {
		return;
	}

	_this->addTabDelegate(mapInTabDelegateRef(env, delegate));
}

extern "C" void BK_Tab_removeTabDelegate(BK_Env* env, BK_Tab self, BK_TabDelegate delegate)
{

	Berkelium::TabRef _this(mapInTabRef(env, self));

	if(!_this) {
		return;
	}

	_this->removeTabDelegate(mapInTabDelegateRef(env, delegate));
}

extern "C" BK_Window BK_Tab_getWindow(BK_Env* env, BK_Tab self)
{

	Berkelium::TabRef _this(mapInTabRef(env, self));

	if(!_this) {
		return NULL;
	}

	return mapOutWindowRef(env, _this->getWindow());
}

extern "C" void BK_Tab_resize(BK_Env* env, BK_Tab self, bk_int32 width, bk_int32 height)
{

	Berkelium::TabRef _this(mapInTabRef(env, self));

	if(!_this) {
		return;
	}

	_this->resize(width, height);
}

extern "C" void BK_Tab_navigateTo(BK_Env* env, BK_Tab self, bk_string url)
{

	Berkelium::TabRef _this(mapInTabRef(env, self));

	if(!_this) {
		return;
	}

	_this->navigateTo(url);
}

extern "C" void BK_TabDelegate_onClosed(BK_Env* env, BK_TabDelegate self, BK_Tab tab)
{

	Berkelium::TabDelegateRef _this(mapInTabDelegateRef(env, self));

	if(!_this) {
		return;
	}

	_this->onClosed(mapInTabRef(env, tab));
}

extern "C" void BK_TabDelegate_onTitleChanged(BK_Env* env, BK_TabDelegate self, BK_Tab tab, bk_string title)
{

	Berkelium::TabDelegateRef _this(mapInTabDelegateRef(env, self));

	if(!_this) {
		return;
	}

	_this->onTitleChanged(mapInTabRef(env, tab), title);
}

extern "C" void BK_TabDelegate_onPaint(BK_Env* env, BK_TabDelegate self, BK_Tab tab)
{

	Berkelium::TabDelegateRef _this(mapInTabDelegateRef(env, self));

	if(!_this) {
		return;
	}

	_this->onPaint(mapInTabRef(env, tab));
}

extern "C" void BK_TabDelegate_onPaintDone(BK_Env* env, BK_TabDelegate self, BK_Tab tab, BK_Rect rect)
{

	Berkelium::TabDelegateRef _this(mapInTabDelegateRef(env, self));

	if(!_this) {
		return;
	}

	_this->onPaintDone(mapInTabRef(env, tab), mapInRectRef(env, rect));
}