#ifdef WINDOWS

#include "berkelium/Impl.hpp"
#include "gtest/gtest.h"

namespace {

class ImplTest : public ::testing::Test {
};

TEST_F(ImplTest, getEnv) {
	std::string key = "SystemDrive";
	std::string value = "C:";
	std::string s = Berkelium::impl::getEnv(key, "");
	ASSERT_EQ(0, s.compare(value));
}

void s2ws(const std::wstring& expected, const std::string& test) {
	std::wstring result = Berkelium::impl::s2ws(test);
	ASSERT_EQ(0, expected.compare(result));
}

void ws2s(const std::string& expected, const std::wstring& test) {
	std::string result = Berkelium::impl::ws2s(test);
	ASSERT_EQ(0, expected.compare(result));
}

TEST_F(ImplTest, s2wsEmpty) {
	s2ws(L"", "");
}

TEST_F(ImplTest, s2wsTest) {
	s2ws(L"test", "test");
}

TEST_F(ImplTest, s2ws) {
	s2ws(L"-äüöÄÜÖß@-", "-äüöÄÜÖß@-");
}

TEST_F(ImplTest, ws2sEmpty) {
	ws2s("", L"");
}

TEST_F(ImplTest, ws2sTest) {
	ws2s("test", L"test");
}

TEST_F(ImplTest, ws2s) {
	ws2s("-äüöÄÜÖß@-", L"-äüöÄÜÖß@-");
}

} // namespace

#endif // WINDOWS
