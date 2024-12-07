#include "tester.hpp"

#include "str.hpp"
#include "view.hpp"

TEST(test_view, trim_xxx) {
    SECTION("左右都没有空白") {
        ASSERT_EQ(view::trim_left("3bc1233"), "3bc1233");
        ASSERT_EQ(view::trim_right("3bc1233"), "3bc1233");
        ASSERT_EQ(view::trim_surrounding("3bc1233"), "3bc1233");
        ASSERT_EQ(view::trim_anywhere("3bc1233"), "3bc1233");
    }

    SECTION("左边有空白") {
        ASSERT_EQ(view::trim_left("\r  \t\nc1233"), "c1233");
        ASSERT_EQ(view::trim_right("\r  \t\nc1233"), "\r  \t\nc1233");
        ASSERT_EQ(view::trim_surrounding("\r  \t\nc1233"), "c1233");
        ASSERT_EQ(view::trim_anywhere("\r  \t\nc1233"), "c1233");
    }

    SECTION("右边有空白") {
        ASSERT_EQ(view::trim_left("c1233\r  \t\n"), "c1233\r  \t\n");
        ASSERT_EQ(view::trim_right("c1233\r  \t\n"), "c1233");
        ASSERT_EQ(view::trim_surrounding("c1233\r  \t\n"), "c1233");
        ASSERT_EQ(view::trim_anywhere("c1233\r  \t\n"), "c1233");
    }

    SECTION("左右两边都有空白") {
        ASSERT_EQ(view::trim_left("\r  \t\nc1233\r  \t\n"), "c1233\r  \t\n");
        ASSERT_EQ(view::trim_right("\r  \t\nc1233\r  \t\n"), "\r  \t\nc1233");
        ASSERT_EQ(view::trim_surrounding("\r  \t\nc1233\r  \t\n"), "c1233");
        ASSERT_EQ(view::trim_anywhere("\r  \t\nc1233\r  \t\n"), "c1233");
    }

    SECTION("中间有空白") {
        ASSERT_EQ(view::trim_left("\r  \t\nc12\r  \t\n33\r  \t\n"), "c12\r  \t\n33\r  \t\n");
        ASSERT_EQ(view::trim_right("\r  \t\nc12\r  \t\n33\r  \t\n"), "\r  \t\nc12\r  \t\n33");
        ASSERT_EQ(view::trim_surrounding("\r  \t\nc12\r  \t\n33\r  \t\n"), "c12\r  \t\n33");
        ASSERT_EQ(view::trim_anywhere("\r  \t\nc12\r  \t\n33\r  \t\n"), "c1233");
    }

    SECTION("空串") {
        ASSERT_EQ(view::trim_left(""), "");
        ASSERT_EQ(view::trim_right(""), "");
        ASSERT_EQ(view::trim_surrounding(""), "");
        ASSERT_EQ(view::trim_anywhere(""), "");
    }

    SECTION("全空白串") {
        ASSERT_EQ(view::trim_left("\r\n \t"), "");
        ASSERT_EQ(view::trim_right("\r\n \t"), "");
        ASSERT_EQ(view::trim_surrounding("\r\n \t"), "");
        ASSERT_EQ(view::trim_anywhere("\r\n \t"), "");
    }

    SECTION("指定字符") {
        ASSERT_TRUE(view::trim_left("33c1233", [](std::string::value_type ch) -> bool {
            return ch == '3';
        }) == "c1233");
        ASSERT_TRUE(view::trim_right("33c1233", [](std::string::value_type ch) -> bool {
            return ch == '3';
        }) == "33c12");
        ASSERT_TRUE(view::trim_surrounding("33c1233", [](std::string::value_type ch) -> bool {
            return ch == '3';
        }) == "c12");
        ASSERT_TRUE(view::trim_anywhere("33c1233", [](std::string::value_type ch) -> bool {
            return ch == '3';
        }) == "c12");

        // 只去掉前三个匹配的字符
        size_t n = 0;
        ASSERT_TRUE(view::trim_anywhere("33c1233", [&n](std::string::value_type ch) -> bool {
            if (n >= 3) {
                return false;
            }

            if (ch == '3') {
                n++;
                return true;
            }

            return false;
        }) == "c123");
    }
}
