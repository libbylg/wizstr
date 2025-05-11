#include "testing.hpp"

#include "str.hpp"

TEST(test_str, trim_anywhere) {
    SECTION("左右都没有空白") {
        ASSERT_EQ(str::trim_anywhere("3bc1233"), "3bc1233");
    }

    SECTION("左边有空白") {
        ASSERT_EQ(str::trim_anywhere("\r  \t\nc1233"), "c1233");
    }

    SECTION("右边有空白") {
        ASSERT_EQ(str::trim_anywhere("c1233\r  \t\n"), "c1233");
    }

    SECTION("左右两边都有空白") {
        ASSERT_EQ(str::trim_anywhere("\r  \t\nc1233\r  \t\n"), "c1233");
    }

    SECTION("中间有空白") {
        ASSERT_EQ(str::trim_anywhere("\r  \t\nc12\r  \t\n33\r  \t\n"), "c1233");
    }

    SECTION("空串") {
        ASSERT_EQ(str::trim_anywhere(""), "");
    }

    SECTION("全空白串") {
        ASSERT_EQ(str::trim_anywhere("\r\n \t"), "");
    }

    SECTION("指定字符") {
        ASSERT_TRUE(str::trim_anywhere("33c1233", [](std::string::value_type ch) -> bool {
            return ch == '3';
        }) == "c12");

        // 只去掉前三个匹配的字符
        size_t n = 0;
        ASSERT_TRUE(str::trim_anywhere("33c1233", [&n](std::string::value_type ch) -> bool {
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
