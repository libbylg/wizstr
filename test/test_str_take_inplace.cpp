#include "testing.hpp"

#include "str.hpp"

TEST(test_str, take_inplace) {
    std::string s;
    SECTION("起始位置:一般场景") {
        ASSERT_EQ(str::take_inplace(s = "abc1234", 3, 9999), "1234");
        ASSERT_EQ(str::take_inplace(s = "abc1234", 3, 5), "1234");
        ASSERT_EQ(str::take_inplace(s = "abc1234", 3, 4), "1234");
        ASSERT_EQ(str::take_inplace(s = "abc1234", 3, 2), "12");
        ASSERT_EQ(str::take_inplace(s = "abc1234", 3, 1), "1");
        ASSERT_EQ(str::take_inplace(s = "abc1234", 3, 0), "");
        ASSERT_EQ(str::take_inplace(s = "abc1234", 3, -1), "1");
        ASSERT_EQ(str::take_inplace(s = "abc1234", 3, -2), "c1");
        ASSERT_EQ(str::take_inplace(s = "abc1234", 3, -4), "abc1");
        ASSERT_EQ(str::take_inplace(s = "abc1234", 3, -5), "abc1");
        ASSERT_EQ(str::take_inplace(s = "abc1234", 3, -999), "abc1");
    }
    SECTION("起始位置:npos") {
        ASSERT_EQ(str::take_inplace(s = "abc1234", str::npos, 6), "4");
        ASSERT_EQ(str::take_inplace(s = "abc1234", str::npos, 1), "4");
        ASSERT_EQ(str::take_inplace(s = "abc1234", str::npos, 0), "");
        ASSERT_EQ(str::take_inplace(s = "abc1234", str::npos, -1), "4");
        ASSERT_EQ(str::take_inplace(s = "abc1234", str::npos, -3), "234");
        ASSERT_EQ(str::take_inplace(s = "abc1234", str::npos, -7), "abc1234");
        ASSERT_EQ(str::take_inplace(s = "abc1234", str::npos, -999), "abc1234");
    }
    SECTION("起始位置:左边界") {
        ASSERT_EQ(str::take_inplace(s = "abc1234", 0, 9999), "abc1234");
        ASSERT_EQ(str::take_inplace(s = "abc1234", 0, 7), "abc1234");
        ASSERT_EQ(str::take_inplace(s = "abc1234", 0, 3), "abc");
        ASSERT_EQ(str::take_inplace(s = "abc1234", 0, 1), "a");
        ASSERT_EQ(str::take_inplace(s = "abc1234", 0, 0), "");
        ASSERT_EQ(str::take_inplace(s = "abc1234", 0, -1), "a");
        ASSERT_EQ(str::take_inplace(s = "abc1234", 0, -3), "a");
        ASSERT_EQ(str::take_inplace(s = "abc1234", 0, -999), "a");
    }
    SECTION("起始位置:右边界") {
        ASSERT_EQ(str::take_inplace(s = "abc1234", 6, 9999), "4");
        ASSERT_EQ(str::take_inplace(s = "abc1234", 6, 1), "4");
        ASSERT_EQ(str::take_inplace(s = "abc1234", 6, 0), "");
        ASSERT_EQ(str::take_inplace(s = "abc1234", 6, -1), "4");
        ASSERT_EQ(str::take_inplace(s = "abc1234", 6, -3), "234");
        ASSERT_EQ(str::take_inplace(s = "abc1234", 6, -7), "3bc1233");
        ASSERT_EQ(str::take_inplace(s = "abc1234", 6, -999), "3bc1233");
    }
    SECTION("空串") {
        ASSERT_EQ(str::take_inplace(s = "", 3, 9999), "");
        ASSERT_EQ(str::take_inplace(s = "", 0, 0), "");
        ASSERT_EQ(str::take_inplace(s = "", 2, -2), "");
    }
    SECTION("无offset场景") {
        ASSERT_EQ(str::take_inplace(s = "3bc1233", 0), "3bc1233");
        ASSERT_EQ(str::take_inplace(s = "3bc1233", 3), "1233");
        ASSERT_EQ(str::take_inplace(s = "3bc1239", 6), "9");
        ASSERT_EQ(str::take_inplace(s = "3bc1234", 7), "");
        ASSERT_EQ(str::take_inplace(s = "3bc1234", str::npos), "");
        ASSERT_EQ(str::take_inplace(s = "", 0), "");
        ASSERT_EQ(str::take_inplace(s = "", 1), "");
        ASSERT_EQ(str::take_inplace(s = "", str::npos), "");
    }
}