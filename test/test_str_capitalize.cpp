#include "tester.hpp"

#include "str.hpp"


TEST(test_str, capitalize) {
    ASSERT_EQ(str::to_capitalize("abc def"), "Abc def");
    ASSERT_EQ(str::to_capitalize("Abc def"), "Abc def");
    ASSERT_EQ(str::to_capitalize("   abc def"), "   abc def");
    ASSERT_EQ(str::to_capitalize("   Abc def"), "   Abc def");
    ASSERT_EQ(str::to_capitalize(""), "");
    ASSERT_EQ(str::to_capitalize("z"), "Z");
    ASSERT_EQ(str::to_capitalize("Q"), "Q");
    ASSERT_EQ(str::to_capitalize("123456"), "123456");
}

TEST(test_str, is_capitalize) {
    ASSERT_EQ(str::is_capitalize("1"), false);
    ASSERT_EQ(str::is_capitalize(""), false);
    ASSERT_EQ(str::is_capitalize("Abc123DEF"), true);
    ASSERT_EQ(str::is_capitalize("abc123DEF"), false);
    ASSERT_EQ(str::is_capitalize("  A"), false);
    ASSERT_EQ(str::is_capitalize("A"), true);
}
