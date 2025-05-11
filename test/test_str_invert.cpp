#include "testing.hpp"

#include "str.hpp"

TEST(test_str, invert) {
    SECTION("反转偶数长度的字符串") {
        ASSERT_EQ(str::invert("0123456789"), "9876543210");
    }
    SECTION("反转奇数长度的字符串") {
        ASSERT_EQ(str::invert("1234567"), "7654321");
    }
    SECTION("反转空字符串") {
        ASSERT_EQ(str::invert(""), "");
    }
    SECTION("反转单字符串") {
        ASSERT_EQ(str::invert("\n"), "\n");
    }
    SECTION("反转范围在字符串内部:起点和终点都有效") {
        ASSERT_EQ(str::invert("0123456789", 3, 5), "0127654389");
        ASSERT_EQ(str::invert("0123456789", 3, 7), "0129876543");
    }
    SECTION("反转范围在字符串外:起始点超出字符串右边界") {
        ASSERT_EQ(str::invert("1234567", 7, 2), "1234567");
        ASSERT_EQ(str::invert("1234567", 10, 2), "1234567");
        ASSERT_EQ(str::invert("1234567", str::npos, 2), "1234567");
    }
    SECTION("反转空字符串") {
        ASSERT_EQ(str::invert("", 0, 1), "");
        ASSERT_EQ(str::invert("", 1, 100), "");
    }
    SECTION("指定反转串的数量太大，超出原串长度") {
        ASSERT_EQ(str::invert("0123456789", 3, 7), "0129876543");
        ASSERT_EQ(str::invert("0123456789", 3, 100), "0129876543");
        ASSERT_EQ(str::invert("0123456789", 3, str::npos), "0129876543");
    }
    SECTION("反转串的长度为0或者1：无反转效果") {
        ASSERT_EQ(str::invert("0123456789", 3, 0), "0123456789");
        ASSERT_EQ(str::invert("0123456789", 3, 1), "0123456789");
        ASSERT_EQ(str::invert("0123456789", 20, 0), "0123456789");
        ASSERT_EQ(str::invert("0123456789", 20, 1), "0123456789");
        ASSERT_EQ(str::invert("0123456789", str::npos, 0), "0123456789");
        ASSERT_EQ(str::invert("0123456789", str::npos, 1), "0123456789");
        ASSERT_EQ(str::invert("", 0, 0), "");
        ASSERT_EQ(str::invert("", 0, 1), "");
        ASSERT_EQ(str::invert("", str::npos, 0), "");
        ASSERT_EQ(str::invert("", str::npos, 1), "");
        ASSERT_EQ(str::invert("C", 0, 0), "C");
        ASSERT_EQ(str::invert("C", 0, 1), "C");
        ASSERT_EQ(str::invert("C", str::npos, 0), "C");
        ASSERT_EQ(str::invert("C", str::npos, 1), "C");
    }
}