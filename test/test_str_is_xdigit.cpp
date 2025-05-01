#include "testing.hpp"

#include "str.hpp"


TEST(test_str, is_xdigit) {
    ASSERT_EQ(str::is_xdigit("0123456789ABCDEF"), true);
    ASSERT_EQ(str::is_xdigit("ABCDEF"), true);
    ASSERT_EQ(str::is_xdigit("123DEF"), true);

    ASSERT_EQ(str::is_xdigit("0123456789abcdefABCDEF"), true);
    ASSERT_EQ(str::is_xdigit("abcdefABCDEF"), true);
    ASSERT_EQ(str::is_xdigit("123deF"), true);

    ASSERT_EQ(str::is_xdigit("ABCDEFGHIJ"), false);
    ASSERT_EQ(str::is_xdigit("abcDEFghij"), false);
    ASSERT_EQ(str::is_xdigit("123GHI"), false);
    ASSERT_EQ(str::is_xdigit(""), false);
    ASSERT_EQ(str::is_xdigit("123 "), false);
    ASSERT_EQ(str::is_xdigit("  123"), false);
}
