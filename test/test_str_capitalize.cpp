#include "tester.hpp"

#include "str.hpp"
#include "view.hpp"

TEST(test_view, capitalize) {
    ASSERT_EQ(view::capitalize("abc def"), "Abc def");
    ASSERT_EQ(view::capitalize("Abc def"), "Abc def");
    ASSERT_EQ(view::capitalize("   abc def"), "   abc def");
    ASSERT_EQ(view::capitalize("   Abc def"), "   Abc def");
    ASSERT_EQ(view::capitalize(""), "");
    ASSERT_EQ(view::capitalize("z"), "Z");
    ASSERT_EQ(view::capitalize("Q"), "Q");
    ASSERT_EQ(view::capitalize("123456"), "123456");
}

TEST(test_view, is_capitalize) {
    ASSERT_EQ(view::is_capitalize("1"), false);
    ASSERT_EQ(view::is_capitalize(""), false);
    ASSERT_EQ(view::is_capitalize("Abc123DEF"), true);
    ASSERT_EQ(view::is_capitalize("abc123DEF"), false);
    ASSERT_EQ(view::is_capitalize("  A"), false);
    ASSERT_EQ(view::is_capitalize("A"), true);
}
