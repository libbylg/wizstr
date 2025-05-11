#include "testing.hpp"

#include "str.hpp"

TEST(test_str, trim_surrounding_view) {
    SECTION("左右都没有空白") {
        ASSERT_EQ(str::trim_surrounding_view("3bc1233"), "3bc1233");
    }

    SECTION("左边有空白") {
        ASSERT_EQ(str::trim_surrounding_view("\r  \t\nc1233"), "c1233");
    }

    SECTION("右边有空白") {
        ASSERT_EQ(str::trim_surrounding_view("c1233\r  \t\n"), "c1233");
    }

    SECTION("左右两边都有空白") {
        ASSERT_EQ(str::trim_surrounding_view("\r  \t\nc1233\r  \t\n"), "c1233");
    }

    SECTION("中间有空白") {
        ASSERT_EQ(str::trim_surrounding_view("\r  \t\nc12\r  \t\n33\r  \t\n"), "c12\r  \t\n33");
    }

    SECTION("空串") {
        ASSERT_EQ(str::trim_surrounding_view(""), "");
    }

    SECTION("全空白串") {
        ASSERT_EQ(str::trim_surrounding_view("\r\n \t"), "");
    }

    SECTION("指定字符") {
        ASSERT_TRUE(str::trim_surrounding_view("33c1233", [](std::string::value_type ch) -> bool {
            return ch == '3';
        }) == "c12");
    }
}
