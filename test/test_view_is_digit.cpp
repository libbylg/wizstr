#include "tester.hpp"

#include "str.hpp"
#include "view.hpp"

TEST(test_view, is_digit) {
    ASSERT_EQ(view::is_digit("0123456789"), true);
    ASSERT_EQ(view::is_digit(""), false);
    ASSERT_EQ(view::is_digit("123DEF"), false);
    ASSERT_EQ(view::is_digit("123 "), false);
    ASSERT_EQ(view::is_digit("  123"), false);
    ASSERT_EQ(view::is_digit("ABCDEF"), false);
}
