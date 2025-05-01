#include "testing.hpp"

#include "str.hpp"


TEST(test_str, has_prefix) {
    SECTION("一般情况") {
        ASSERT_EQ(str::has_prefix("aaa", "aa"), true);
        ASSERT_EQ(str::has_prefix("aaa", "aab"), false);
    }
    SECTION("无共同前缀") {
        ASSERT_EQ(str::has_prefix("aaa", "bb"), false);
    }
    SECTION("完全相同") {
        ASSERT_EQ(str::has_prefix("aaa", "aaa"), true);
    }
    SECTION("部分为空") {
        ASSERT_EQ(str::has_prefix("", "aaa"), false);
        ASSERT_EQ(str::has_prefix("aaa", ""), true);
    }
    SECTION("空对空") {
        ASSERT_EQ(str::has_prefix("", ""), true);
    }
    SECTION("包含关系") {
        ASSERT_EQ(str::has_prefix("aaa", "aa"), true);
    }
    SECTION("空字符串找前缀") {
        ASSERT_EQ(str::has_prefix("", "Hello"), false);
        ASSERT_EQ(str::has_prefix("Hello", ""), true);
        ASSERT_EQ(str::has_prefix("", ""), true);
    }
    SECTION("字符前缀") {
        ASSERT_EQ(str::has_prefix("HelloWorld", 'H'), true);
        ASSERT_EQ(str::has_prefix("HelloWorld", 'W'), false);
        ASSERT_EQ(str::has_prefix("", 'H'), false);
        ASSERT_EQ(str::has_prefix("", '\0'), false);
    }
}
