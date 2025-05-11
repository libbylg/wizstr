#include "testing.hpp"

#include "str.hpp"

TEST(test_str, to_capitalize_inplace) {
    std::string s;
    ASSERT_EQ(str::to_capitalize_inplace(s = "abc def"), "Abc def");
    ASSERT_EQ(str::to_capitalize_inplace(s = "Abc def"), "Abc def");
    ASSERT_EQ(str::to_capitalize_inplace(s = "   abc def"), "   abc def");
    ASSERT_EQ(str::to_capitalize_inplace(s = "   Abc def"), "   Abc def");
    ASSERT_EQ(str::to_capitalize_inplace(s = ""), "");
    ASSERT_EQ(str::to_capitalize_inplace(s = "z"), "Z");
    ASSERT_EQ(str::to_capitalize_inplace(s = "Q"), "Q");
    ASSERT_EQ(str::to_capitalize_inplace(s = "123456"), "123456");
}
