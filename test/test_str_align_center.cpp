#include "testing.hpp"

#include "str.hpp"


TEST(test_str, align_center) {
    SECTION("一般情况：字符串居中对齐对齐，两边填充") {
        ASSERT_EQ(str::align_center("HelloWorld", 13, '*'), "*HelloWorld**");
        ASSERT_EQ(str::align_center("HelloWorld", 14, '*'), "**HelloWorld**");
        ASSERT_EQ(str::align_center("Hello", 13, '*'), "****Hello****");
        ASSERT_EQ(str::align_center("Hello", 14, '*'), "****Hello*****");
    }

    SECTION("宽度和原字符串相同：字符串不变") {
        ASSERT_EQ(str::align_center("HelloWorld", 10, '*'), "HelloWorld");
    }

    SECTION("宽度太窄时保持原样") {
        ASSERT_EQ(str::align_center("HelloWorld", 9, '*'), "HelloWorld");
    }

    SECTION("空字符串") {
        ASSERT_EQ(str::align_center("", 9, '*'), "*********");
    }

    SECTION("对齐宽度为0") {
        ASSERT_EQ(str::align_center("HelloWorld", 0, '*'), "HelloWorld");
        ASSERT_EQ(str::align_center("", 0, '*'), "");
    }
}
