#include "testing.hpp"

#include "str.hpp"

TEST(test_str, trim_right) {
    SECTION("左右都没有空白") {
        ASSERT_EQ(str::trim_right("3bc1233"), "3bc1233");
    }

    SECTION("左边有空白") {
        ASSERT_EQ(str::trim_right("\r  \t\nc1233"), "\r  \t\nc1233");
    }

    SECTION("右边有空白") {
        ASSERT_EQ(str::trim_right("c1233\r  \t\n"), "c1233");
    }

    SECTION("左右两边都有空白") {
        ASSERT_EQ(str::trim_right("\r  \t\nc1233\r  \t\n"), "\r  \t\nc1233");
    }

    SECTION("中间有空白") {
        ASSERT_EQ(str::trim_right("\r  \t\nc12\r  \t\n33\r  \t\n"), "\r  \t\nc12\r  \t\n33");
    }

    SECTION("空串") {
        ASSERT_EQ(str::trim_right(""), "");
    }

    SECTION("全空白串") {
        ASSERT_EQ(str::trim_right("\r\n \t"), "");
    }

    SECTION("指定字符") {
        ASSERT_TRUE(str::trim_right("33c1233", [](std::string::value_type ch) -> bool {
            return ch == '3';
        }) == "33c12");
    }

    SECTION("指定字符集") {
        ASSERT_EQ(str::trim_right("1234ABC", str::charset(str::all_uppers)), "1234");
        ASSERT_EQ(str::trim_right("1234ABC", str::all_alphas), "1234");
        ASSERT_EQ(str::trim_right("1234ABC", ""), "1234ABC");
        ASSERT_EQ(str::trim_right("", ""), "");
    }
}
