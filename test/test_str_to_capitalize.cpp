#include "testing.hpp"

#include "str.hpp"

TEST(test_str, to_capitalize) {
    ASSERT_EQ(str::to_capitalize("abc def"), "Abc def");
    ASSERT_EQ(str::to_capitalize("Abc def"), "Abc def");
    ASSERT_EQ(str::to_capitalize("   abc def"), "   abc def");
    ASSERT_EQ(str::to_capitalize("   Abc def"), "   Abc def");
    ASSERT_EQ(str::to_capitalize(""), "");
    ASSERT_EQ(str::to_capitalize("z"), "Z");
    ASSERT_EQ(str::to_capitalize("Q"), "Q");
    ASSERT_EQ(str::to_capitalize("123456"), "123456");
}
