#include "testing.hpp"

#include "str.hpp"

TEST(test_str, remove_prefix) {
    SECTION("一般情况") {
        ASSERT_EQ(str::remove_prefix("aac", "aa"), "c");
        ASSERT_EQ(str::remove_prefix("aaa", "aab"), "aaa");
    }
    SECTION("无共同前缀") {
        ASSERT_EQ(str::remove_prefix("aaa", "bb"), "aaa");
    }
    SECTION("完全相同") {
        ASSERT_EQ(str::remove_prefix("aaa", "aaa"), "");
    }
    SECTION("部分为空") {
        ASSERT_EQ(str::remove_prefix("", "aaa"), "");
        ASSERT_EQ(str::remove_prefix("aaa", ""), "aaa");
    }
    SECTION("空对空") {
        ASSERT_EQ(str::remove_prefix("", ""), "");
    }
    SECTION("包含关系") {
        ASSERT_EQ(str::remove_prefix("aac", "aa"), "c");
    }
    SECTION("空字符串找前缀") {
        ASSERT_EQ(str::remove_prefix("", "Hello"), "");
        ASSERT_EQ(str::remove_prefix("Hello", ""), "Hello");
        ASSERT_EQ(str::remove_prefix("", ""), "");
    }
    SECTION("字符前缀") {
        ASSERT_EQ(str::remove_prefix("HelloWorld", 'H'), "elloWorld");
        ASSERT_EQ(str::remove_prefix("HelloWorld", 'W'), "HelloWorld");
        ASSERT_EQ(str::remove_prefix("", 'H'), "");
        ASSERT_EQ(str::remove_prefix("", '\0'), "");
    }
}

