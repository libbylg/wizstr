#include "testing.hpp"

#include "str.hpp"

TEST(test_str, remove_prefix_view) {
    SECTION("一般情况") {
        ASSERT_EQ(str::remove_prefix_view("aac", "aa"), "c");
        ASSERT_EQ(str::remove_prefix_view("aaa", "aab"), "aaa");
    }
    SECTION("无共同前缀") {
        ASSERT_EQ(str::remove_prefix_view("aaa", "bb"), "aaa");
    }
    SECTION("完全相同") {
        ASSERT_EQ(str::remove_prefix_view("aaa", "aaa"), "");
    }
    SECTION("部分为空") {
        ASSERT_EQ(str::remove_prefix_view("", "aaa"), "");
        ASSERT_EQ(str::remove_prefix_view("aaa", ""), "aaa");
    }
    SECTION("空对空") {
        ASSERT_EQ(str::remove_prefix_view("", ""), "");
    }
    SECTION("包含关系") {
        ASSERT_EQ(str::remove_prefix_view("aac", "aa"), "c");
    }
    SECTION("空字符串找前缀") {
        ASSERT_EQ(str::remove_prefix_view("", "Hello"), "");
        ASSERT_EQ(str::remove_prefix_view("Hello", ""), "Hello");
        ASSERT_EQ(str::remove_prefix_view("", ""), "");
    }
    SECTION("字符前缀") {
        ASSERT_EQ(str::remove_prefix_view("HelloWorld", 'H'), "elloWorld");
        ASSERT_EQ(str::remove_prefix_view("HelloWorld", 'W'), "HelloWorld");
        ASSERT_EQ(str::remove_prefix_view("", 'H'), "");
        ASSERT_EQ(str::remove_prefix_view("", '\0'), "");
    }
}

