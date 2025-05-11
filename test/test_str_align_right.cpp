#include "testing.hpp"

#include "str.hpp"

TEST(test_str, align_right) {
    SECTION("一般情况：字符串右对齐，左边填充") {
        ASSERT_EQ(str::align_right("HelloWorld", 13, '*'), "***HelloWorld");
    }

    SECTION("宽度和原字符串相同：字符串不变") {
        ASSERT_EQ(str::align_right("HelloWorld", 10, '*'), "HelloWorld");
    }

    SECTION("宽度太窄时保持原样") {
        ASSERT_EQ(str::align_right("HelloWorld", 9, '*'), "HelloWorld");
    }

    SECTION("空字符串") {
        ASSERT_EQ(str::align_right("", 9, '*'), "*********");
    }

    SECTION("对齐宽度为0") {
        ASSERT_EQ(str::align_right("HelloWorld", 0, '*'), "HelloWorld");
        ASSERT_EQ(str::align_right("", 0, '*'), "");
    }
}
