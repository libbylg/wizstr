#include "tester.hpp"

#include "str.hpp"
#include "view.hpp"

TEST(test_view, is_ascii) {
    ASSERT_EQ(view::is_ascii("0123456789ABCDEF#$%\r\n\t\v "), true);
    ASSERT_EQ(view::is_ascii("ABCDEF"), true);
    ASSERT_EQ(view::is_ascii("123DEF"), true);
    ASSERT_EQ(view::is_ascii(std::string_view{"\0\0", 2}), true);
    ASSERT_EQ(view::is_ascii("123你好DEF"), false);
    ASSERT_EQ(view::is_ascii(""), false);
}
