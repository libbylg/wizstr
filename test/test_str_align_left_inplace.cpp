#include "testing.hpp"

#include "str.hpp"

TEST(test_str, align_left_inplace) {
    std::string s;
    SECTION("一般情况：字符串左对齐，右边填充") {
        ASSERT_EQ(str::align_left_inplace(s = "HelloWorld", 13, '*'), "HelloWorld***");
    }

    SECTION("宽度和原字符串相同：字符串不变") {
        ASSERT_EQ(str::align_left_inplace(s = "HelloWorld", 10, '*'), "HelloWorld");
    }

    SECTION("宽度太窄时保持原样") {
        ASSERT_EQ(str::align_left_inplace(s = "HelloWorld", 9, '*'), "HelloWorld");
    }

    SECTION("空字符串") {
        ASSERT_EQ(str::align_left_inplace(s = "", 9, '*'), "*********");
    }

    SECTION("对齐宽度为0") {
        ASSERT_EQ(str::align_left_inplace(s = "HelloWorld", 0, '*'), "HelloWorld");
        ASSERT_EQ(str::align_left_inplace(s = "", 0, '*'), "");
    }
}
