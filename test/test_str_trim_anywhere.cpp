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

    SECTION("proc") {
        ASSERT_TRUE(str::trim_anywhere("", [](std::string::value_type ch) -> bool {
            return std::isdigit(ch);
        }) == "");
        ASSERT_TRUE(str::trim_anywhere("3", [](std::string::value_type ch) -> bool {
            return std::isdigit(ch);
        }) == "");
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
    SECTION("指定字符集") {
        ASSERT_EQ(str::trim_anywhere("Calc: 123 + 456 = 579!", str::charset("0123456789")), "Calc:  +  = !");
        ASSERT_EQ(str::trim_anywhere("123", str::charset("")), "123");
        ASSERT_EQ(str::trim_anywhere("", str::charset("")), "");
        ASSERT_EQ(str::trim_anywhere("", str::charset("0123456789")), "");

        ASSERT_EQ(str::trim_anywhere("Calc: 123 + 456 = 579!", "0123456789"), "Calc:  +  = !");
        ASSERT_EQ(str::trim_anywhere("123", ""), "123");
        ASSERT_EQ(str::trim_anywhere("", ""), "");
        ASSERT_EQ(str::trim_anywhere("", "0123456789"), "");
    }
    SECTION("指定单个字符") {
        ASSERT_EQ(str::trim_anywhere("Calc: 123 + 456 = 579!", '5'), "Calc: 123 + 46 = 79!");
    }
}
