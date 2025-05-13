#include "testing.hpp"

#include "str.hpp"

TEST(test_str, simplified_integer_inplace) {
    std::string s;

    ASSERT_EQ(str::simplified_integer_inplace(s = "123"), "123");
    ASSERT_EQ(str::simplified_integer_inplace(s = "000123"), "123");
    ASSERT_EQ(str::simplified_integer_inplace(s = "+0000123"), "+123");
    ASSERT_EQ(str::simplified_integer_inplace(s = "-0000123"), "-123");
    ASSERT_EQ(str::simplified_integer_inplace(s = "00000"), "0");
    ASSERT_EQ(str::simplified_integer_inplace(s = "+0000"), "+0");
    ASSERT_EQ(str::simplified_integer_inplace(s = "-0000"), "-0");
    ASSERT_EQ(str::simplified_integer_inplace(s = "+0"), "+0");
    ASSERT_EQ(str::simplified_integer_inplace(s = "-0"), "-0");
    ASSERT_EQ(str::simplified_integer_inplace(s = "0"), "0");
    ASSERT_EQ(str::simplified_integer_inplace(s = "000  "), "0  ");
    ASSERT_EQ(str::simplified_integer_inplace(s = "abc"), "abc");
    ASSERT_EQ(str::simplified_integer_inplace(s = "  000123"), "  000123");
    ASSERT_EQ(str::simplified_integer_inplace(s = "+abc"), "+abc");
    ASSERT_EQ(str::simplified_integer_inplace(s = "+"), "+");
    ASSERT_EQ(str::simplified_integer_inplace(s = ""), "");
}
