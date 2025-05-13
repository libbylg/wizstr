#include "testing.hpp"

#include "str.hpp"

TEST(test_str, simplified_integer) {

    ASSERT_EQ(str::simplified_integer("123"), "123");
    ASSERT_EQ(str::simplified_integer("000123"), "123");
    ASSERT_EQ(str::simplified_integer("+0000123"), "+123");
    ASSERT_EQ(str::simplified_integer("-0000123"), "-123");
    ASSERT_EQ(str::simplified_integer("00000"), "0");
    ASSERT_EQ(str::simplified_integer("+0000"), "+0");
    ASSERT_EQ(str::simplified_integer("-0000"), "-0");

    ASSERT_EQ(str::simplified_integer("000  "), "0  ");
    ASSERT_EQ(str::simplified_integer("abc"), "abc");
    ASSERT_EQ(str::simplified_integer("  000123"), "  000123");
    ASSERT_EQ(str::simplified_integer("+abc"), "+abc");
    ASSERT_EQ(str::simplified_integer("+"), "+");
    ASSERT_EQ(str::simplified_integer(""), "");
}
