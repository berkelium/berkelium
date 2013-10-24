// Copyright (c) 2013 The Berkelium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifdef BERKELIUM_TRACE_ALL_OBJECTS

#include <Berkelium/Impl/Impl.hpp>

#include <map>
#include <sstream>

namespace Berkelium {

namespace impl {

std::map<std::string, int>* objectTracerCounterMap = NULL;

void updateCount(const char* name, bool up)
{
	if(objectTracerCounterMap == NULL) {
		objectTracerCounterMap = new std::map<std::string, int>();
	}

	std::map<std::string, int>::iterator it(objectTracerCounterMap->find(name));
	if(it == objectTracerCounterMap->end()) {
		objectTracerCounterMap->insert(std::pair<std::string, int>(name, up ? 1 : -1));
	} else {
		it->second += up ? 1 : -1;
	}

	if(objectTracerCounterMap->empty()) {
		delete objectTracerCounterMap;
		objectTracerCounterMap = NULL;
	}
}

std::string getBerkeliumObjectCount()
{
	if(objectTracerCounterMap == NULL) {
		return "";
	}

	std::stringstream ss;

	for(std::map<std::string, int>::iterator it(objectTracerCounterMap->begin()); it != objectTracerCounterMap->end(); it++) {
		if(it->second != 0) {
			ss << it->first << ' ' << it->second << "\n";
		}
	}

	return ss.str();
}

} // namespace impl

} // namespace Berkelium

#endif // BERKELIUM_TRACE_ALL_OBJECTS
