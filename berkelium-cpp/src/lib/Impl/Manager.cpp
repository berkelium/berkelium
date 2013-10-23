// Copyright (c) 2013 The Berkelium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Berkelium/API/Runtime.hpp>
#include <Berkelium/Impl/Manager.hpp>
#include <Berkelium/Impl/Impl.hpp>

#include <map>
#include <sstream>

namespace Berkelium {


namespace impl {

Manager::Manager() {
}

Manager::~Manager() {
}

template <typename T>
class TypeManager {
	typedef std::map<void*, std::weak_ptr<T>> MapT;
	typedef typename MapT::iterator MapIt;

	MapT map;
	std::string name;

public:
	TypeManager(const char* name) :
		map(),
		name(name) {
	}

	void toString(std::stringstream& ss) {
		if(map.empty()) {
			return;
		}
		ss << name << " " << map.size() << "\n";
	}

	void registerRef(std::shared_ptr<T> ref, ManagerWRef manager) {
		map.insert(std::pair<void*, std::weak_ptr<T>>(ref.get(), ref));
	}

	void unregisterRef() {
		getRef(NULL);
	}

	std::shared_ptr<T> getRef(void* id) {
		for(MapIt it(map.begin()); it != map.end();) {
			if(it->second.expired()) {
				it = map.erase(it);
			} else if(it->first == id){
				return it->second.lock();
			} else {
				it++;
			}
		}
		return std::shared_ptr<T>();
	}
};

class ManagerImpl : public Manager {
private:
	ManagerWRef self;

	LogDelegateRef logger;

#define ALL_TYPES(X) TypeManager<X> X##Manager;
#include "AllTypes.hpp"
#undef ALL_TYPES

	std::map<void*, void*> lockedMap;

	ManagerImpl(const ManagerImpl&);
	void operator=(const ManagerImpl&);

public:
	inline void setSelf(ManagerWRef _self) {
		self = _self;
	}

	ManagerImpl(LogDelegateRef logger) :
		Manager(),
		self(),
		logger(logger),

#define ALL_TYPES(X) X##Manager(#X),
#include "AllTypes.hpp"
#undef ALL_TYPES

		lockedMap() {
	}

	virtual ~ManagerImpl() {
	}

#define FUNCTION_NAME(X, Y) X ## Y
#define ALL_TYPES(X)                                        \
 virtual void FUNCTION_NAME(register,X)(X##Ref ref) {     \
  X##Manager.registerRef(ref, self);                         \
 }                                                           \
                                                             \
 virtual void FUNCTION_NAME(unregister,X)() {               \
  X##Manager.unregisterRef();                                \
 }                                                           \
                                                             \
 virtual X##Ref FUNCTION_NAME(get,X)(void* id) {           \
  return X##Manager.getRef(id);                             \
 }

#include "AllTypes.hpp"
#undef ALL_TYPES

	virtual bool locked(void* id) {
		return lockedMap.find(id) != lockedMap.end();
	}

	virtual void lock(void* id, void* obj) {
		if(locked(id)) {
			return;
		}
		lockedMap.insert(std::pair<void*, void*>(id, obj));
	}

	virtual void* unlock(void* id) {
		std::map<void*, void*>::iterator it(lockedMap.find(id));
		if(it == lockedMap.end()) {
			bk_error("not locked %p!\n", id);
			return NULL;
		}
		lockedMap.erase(it);
		return it->second;
	}

	virtual std::string toString() {
		std::stringstream ret;

#define ALL_TYPES(X) X##Manager.toString(ret);
#include "AllTypes.hpp"
#undef ALL_TYPES

		return ret.str();
	}
};

ManagerRef newManager(LogDelegateRef logger)
{
	ManagerImpl* impl = new ManagerImpl(logger);
	ManagerRef ret(impl);
	impl->setSelf(ret);
	return ret;
}

} // namespace impl

} // namespace Berkelium
