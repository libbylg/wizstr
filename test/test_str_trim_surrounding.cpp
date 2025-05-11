#include "testing.hpp"

#include "str.hpp"

TEST(test_str, trim_surrounding) {
    SECTION("左右都没有空白") {
        ASSERT_EQ(str::trim_surrounding("3bc1233"), "3bc1233");
    }

    SECTION("左边有空白") {
        ASSERT_EQ(str::trim_surrounding("\r  \t\nc1233"), "c1233");
    }

    SECTION("右边有空白") {
        ASSERT_EQ(str::trim_surrounding("c1233\r  \t\n"), "c1233");
    }

    SECTION("左右两边都有空白") {
        ASSERT_EQ(str::trim_surrounding("\r  \t\nc1233\r  \t\n"), "c1233");
    }

    SECTION("中间有空白") {
        ASSERT_EQ(str::trim_surrounding("\r  \t\nc12\r  \t\n33\r  \t\n"), "c12\r  \t\n33");
    }

    SECTION("空串") {
        ASSERT_EQ(str::trim_surrounding(""), "");
    }

    SECTION("全空白串") {
        ASSERT_EQ(str::trim_surrounding("\r\n \t"), "");
    }

    SECTION("指定字符") {
        ASSERT_TRUE(str::trim_surrounding("33c1233", [](std::string::value_type ch) -> bool {
            return ch == '3';
        }) == "c12");
    }

    SECTION("指定字符集") {
        ASSERT_EQ(str::trim_surrounding("1233ABC000", str::charset(str::all_digits)), "ABC");
        ASSERT_EQ(str::trim_surrounding("1233ABC000", str::all_digits), "ABC");
        ASSERT_EQ(str::trim_surrounding("1233ABC000", ""), "1233ABC000");
        ASSERT_EQ(str::trim_surrounding("", ""), "");
    }
}
