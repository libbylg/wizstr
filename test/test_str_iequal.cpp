#include "tester.hpp"

#include "str.hpp"
#include "view.hpp"


TEST(test_view, iequals) {
    SECTION("同长字符串对比") {
        ASSERT_EQ(view::iequals("ABCdef", "abcdef"), true);
        ASSERT_EQ(view::iequals("ABCDEF", "ABCDEF"), true);
        ASSERT_EQ(view::iequals("abcdef", "ABCDEF"), true);
        ASSERT_EQ(view::iequals("01234$%#$%#$^", "01234$%#$%#$^"), true);
        ASSERT_EQ(view::iequals("abcdef", "ABC123"), false);
    }
    SECTION("长短不一的对比") {
        ASSERT_EQ(view::iequals("abc", "ABCdef"), false);
        ASSERT_EQ(view::iequals("ABCdef", "abc"), false);
    }
    SECTION("空串") {
        ASSERT_EQ(view::iequals("", ""), true);
        ASSERT_EQ(view::iequals("", "abc"), false);
        ASSERT_EQ(view::iequals("abc", ""), false);
    }
    SECTION("指定最大对比长度") {
        ASSERT_EQ(view::iequals("", "", 0), true);
        ASSERT_EQ(view::iequals("", "abc", 1), false);
        ASSERT_EQ(view::iequals("abc", "", 1), false);
        ASSERT_EQ(view::iequals("abc", "abc", 0), true);
        ASSERT_EQ(view::iequals("abc", "ABCdef", 3), true);
        ASSERT_EQ(view::iequals("ABCdef", "abc", 4), false);
        ASSERT_EQ(view::iequals("ABCdef", "abcDEF", 6), true);
        ASSERT_EQ(view::iequals("ABCdef", "abcDEF", 7), true);
        ASSERT_EQ(view::iequals("ABCdef", "abcDEF", view::npos), true);
    }
}