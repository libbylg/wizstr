#include "testing.hpp"

#include "str.hpp"

TEST(test_str, trim_left_view) {
    SECTION("左右都没有空白") {
        ASSERT_EQ(str::trim_left_view("3bc1233"), "3bc1233");
    }

    SECTION("左边有空白") {
        ASSERT_EQ(str::trim_left_view("\r  \t\nc1233"), "c1233");
    }

    SECTION("右边有空白") {
        ASSERT_EQ(str::trim_left_view("c1233\r  \t\n"), "c1233\r  \t\n");
    }

    SECTION("左右两边都有空白") {
        ASSERT_EQ(str::trim_left_view("\r  \t\nc1233\r  \t\n"), "c1233\r  \t\n");
    }

    SECTION("中间有空白") {
        ASSERT_EQ(str::trim_left_view("\r  \t\nc12\r  \t\n33\r  \t\n"), "c12\r  \t\n33\r  \t\n");
    }

    SECTION("空串") {
        ASSERT_EQ(str::trim_left_view(""), "");
    }

    SECTION("全空白串") {
        ASSERT_EQ(str::trim_left_view("\r\n \t"), "");
    }

    SECTION("指定字符") {
        ASSERT_TRUE(str::trim_left_view("33c1233", [](std::string::value_type ch) -> bool {
            return ch == '3';
        }) == "c1233");
    }

    SECTION("指定字符集") {
        ASSERT_EQ(str::trim_left_view("1233ABC\r  \t\n", str::charset(str::all_digits)), "ABC\r  \t\n");
        ASSERT_EQ(str::trim_left_view("1233ABC\r  \t\n", str::all_digits), "ABC\r  \t\n");
        ASSERT_EQ(str::trim_left_view("1233ABC\r  \t\n", ""), "1233ABC\r  \t\n");
        ASSERT_EQ(str::trim_left_view("", ""), "");
    }
}
