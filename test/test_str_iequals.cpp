#include "tester.hpp"

#include "str.hpp"



TEST(test_str, iequals) {
    SECTION("同长字符串对比") {
        ASSERT_EQ(str::iequals("ABCdef", "abcdef"), true);
        ASSERT_EQ(str::iequals("ABCDEF", "ABCDEF"), true);
        ASSERT_EQ(str::iequals("abcdef", "ABCDEF"), true);
        ASSERT_EQ(str::iequals("01234$%#$%#$^", "01234$%#$%#$^"), true);
        ASSERT_EQ(str::iequals("abcdef", "ABC123"), false);
    }
    SECTION("长短不一的对比") {
        ASSERT_EQ(str::iequals("abc", "ABCdef"), false);
        ASSERT_EQ(str::iequals("abM", "ABCdef"), false);
        ASSERT_EQ(str::iequals("ABCdef", "abc"), false);
        ASSERT_EQ(str::iequals("ABMdef", "abc"), false);
    }
    SECTION("空串") {
        ASSERT_EQ(str::iequals("", ""), true);
        ASSERT_EQ(str::iequals("", "abc"), false);
        ASSERT_EQ(str::iequals("abc", ""), false);
    }
    SECTION("指定最大对比长度") {
        ASSERT_EQ(str::iequals("", "", 0), true);
        ASSERT_EQ(str::iequals("", "abc", 1), false);
        ASSERT_EQ(str::iequals("abc", "", 1), false);
        ASSERT_EQ(str::iequals("abc", "abc", 0), true);
        ASSERT_EQ(str::iequals("abc", "ABCdef", 3), true);
        ASSERT_EQ(str::iequals("ABCdef", "abc", 4), false);
        ASSERT_EQ(str::iequals("ABCdef", "abcDEF", 6), true);
        ASSERT_EQ(str::iequals("ABCdef", "abcDEF", 7), true);
        ASSERT_EQ(str::iequals("ABCdef", "abcDEF", str::npos), true);
    }
}