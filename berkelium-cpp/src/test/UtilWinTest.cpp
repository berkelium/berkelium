// Copyright (c) 2012 The Berkelium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifdef WINDOWS

#include <Berkelium/API/Util.hpp>
#include "gtest/gtest.h"

namespace {

class UtilWinTest : public ::testing::Test {
};

TEST_F(UtilWinTest, getEnv) {
	std::string key = "SystemDrive";
	std::string value = "C:";
	std::string s = Berkelium::Util::getEnv(key, "");
	ASSERT_EQ(0, s.compare(value));
}

void s2ws(const std::wstring& expected, const std::string& test) {
	std::wstring result = Berkelium::Util::s2ws(test);
	ASSERT_EQ(0, expected.compare(result));
}

void ws2s(const std::string& expected, const std::wstring& test) {
	std::string result = Berkelium::Util::ws2s(test);
	ASSERT_EQ(0, expected.compare(result));
}

TEST_F(UtilWinTest, s2wsEmpty) {
	s2ws(L"", "");
}

TEST_F(UtilWinTest, s2wsTest) {
	s2ws(L"test", "test");
}

TEST_F(UtilWinTest, s2ws) {
	s2ws(L"-äüöÄÜÖß@-", "-äüöÄÜÖß@-");
}

TEST_F(UtilWinTest, ws2sEmpty) {
	ws2s("", L"");
}

TEST_F(UtilWinTest, ws2sTest) {
	ws2s("test", L"test");
}

TEST_F(UtilWinTest, ws2s) {
	ws2s("-äüöÄÜÖß@-", L"-äüöÄÜÖß@-");
}

} // namespace

#endif // WINDOWS
