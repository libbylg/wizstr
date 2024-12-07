#include "tester.hpp"

#include "str.hpp"
#include "view.hpp"

TEST(test_view, expand_tabs) {
    SECTION("简单场景") {
        ASSERT_EQ(view::expand_tabs("1\t123\t1234\t12345\t123456\t$", 4), "1   123 1234    12345   123456  $");
    }
    SECTION("空串") {
        ASSERT_EQ(view::expand_tabs("", 4), "");
    }
    SECTION("tab位于tab_size边界") {
        ASSERT_EQ(view::expand_tabs("\t12345678\t9", 8), "        12345678        9");
    }
    SECTION("不同位置的tab") {
        // clang-format off
        ASSERT_EQ(view::expand_tabs("1\t9", 4),     "1   9");
        ASSERT_EQ(view::expand_tabs("12\t9", 4),    "12  9");
        ASSERT_EQ(view::expand_tabs("123\t9", 4),   "123 9");
        ASSERT_EQ(view::expand_tabs("1234\t9", 4),  "1234    9");
        ASSERT_EQ(view::expand_tabs("12345\t9", 4), "12345   9");
        // clang-format on
    }
    SECTION("不同宽度") {
        ASSERT_EQ(view::expand_tabs("1\t9", 0), "1       9");
        ASSERT_EQ(view::expand_tabs("1\t9", 1), "1 9");
        ASSERT_EQ(view::expand_tabs("1\t9", 2), "1 9");
        ASSERT_EQ(view::expand_tabs("1\t9", 3), "1  9");
        ASSERT_EQ(view::expand_tabs("1\t9", 4), "1   9");
        ASSERT_EQ(view::expand_tabs("1\t9", 5), "1    9");
        ASSERT_EQ(view::expand_tabs("1\t9", 6), "1     9");
        ASSERT_EQ(view::expand_tabs("1\t9", 7), "1      9");
        ASSERT_EQ(view::expand_tabs("1\t9", 8), "1       9");
        ASSERT_EQ(view::expand_tabs("1\t9", 9), "1        9");
    }
}
