#include "tester.hpp"

#include "str.hpp"


TEST(test_str, suffix) {
    SECTION("一般情况") {
        ASSERT_EQ(str::suffix("aaa", "baa"), 2);
    }
    SECTION("无共同后缀") {
        ASSERT_EQ(str::suffix("aaa", "bbb"), 0);
    }
    SECTION("完全相同") {
        ASSERT_EQ(str::suffix("aaa", "aaa"), 3);
    }
    SECTION("部分为空") {
        ASSERT_EQ(str::suffix("", "aaa"), 0);
        ASSERT_EQ(str::suffix("aaa", ""), 0);
    }
    SECTION("空对空") {
        ASSERT_EQ(str::suffix("", ""), 0);
    }
    SECTION("包含关系") {
        ASSERT_EQ(str::suffix("aaa", "aa"), 2);
        ASSERT_EQ(str::suffix("aa", "aaa"), 2);
    }
}
