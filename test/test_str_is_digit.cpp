#include "testing.hpp"

#include "str.hpp"


TEST(test_str, is_digit) {
    ASSERT_EQ(str::is_digit("0123456789"), true);
    ASSERT_EQ(str::is_digit(""), false);
    ASSERT_EQ(str::is_digit("123DEF"), false);
    ASSERT_EQ(str::is_digit("123 "), false);
    ASSERT_EQ(str::is_digit("  123"), false);
    ASSERT_EQ(str::is_digit("ABCDEF"), false);
}
