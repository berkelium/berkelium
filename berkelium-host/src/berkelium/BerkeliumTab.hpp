// Copyright (c) 2012 The Berkelium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BERKELIUMTAB_HPP_
#define BERKELIUMTAB_HPP_

#include <string>

namespace Berkelium {

class BerkeliumTab {
public:
	BerkeliumTab();
	virtual ~BerkeliumTab() = 0;

	// to window
	virtual void Close() = 0;
	virtual void Update() = 0;
	virtual void NavigateTo(const std::string& url) = 0;

	// from window
	virtual void OnReady() = 0;
	virtual void OnLoading(bool start) = 0;

private:
	BerkeliumTab(const BerkeliumTab&);
	void operator=(const BerkeliumTab&);
};

} // namespace Berkelium

#endif /* BERKELIUMTAB_HPP_ */
