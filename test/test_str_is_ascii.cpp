#include "testing.hpp"

#include "str.hpp"


TEST(test_str, is_ascii) {
    ASSERT_EQ(str::is_ascii("0123456789ABCDEF#$%\r\n\t\v "), true);
    ASSERT_EQ(str::is_ascii("ABCDEF"), true);
    ASSERT_EQ(str::is_ascii("123DEF"), true);
    ASSERT_EQ(str::is_ascii(std::string_view{"\0\0", 2}), true);
    ASSERT_EQ(str::is_ascii("123你好DEF"), false);
    ASSERT_EQ(str::is_ascii(""), false);
}
