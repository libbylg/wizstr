#include "testing.hpp"

#include "str.hpp"

TEST(test_str, remove_prefix_inplace) {
    SECTION("一般情况") {
        std::string s;
        ASSERT_EQ(str::remove_prefix_inplace(s = "aac", "aa"), "c");
        ASSERT_EQ(str::remove_prefix_inplace(s = "aaa", "aab"), "aaa");
    }
    SECTION("无共同前缀") {
        std::string s;
        ASSERT_EQ(str::remove_prefix_inplace(s = "aaa", "bb"), "aaa");
    }
    SECTION("完全相同") {
        std::string s;
        ASSERT_EQ(str::remove_prefix_inplace(s = "aaa", "aaa"), "");
    }
    SECTION("部分为空") {
        std::string s;
        ASSERT_EQ(str::remove_prefix_inplace(s = "", "aaa"), "");
        ASSERT_EQ(str::remove_prefix_inplace(s = "aaa", ""), "aaa");
    }
    SECTION("空对空") {
        std::string s;
        ASSERT_EQ(str::remove_prefix_inplace(s = "", ""), "");
    }
    SECTION("包含关系") {
        std::string s;
        ASSERT_EQ(str::remove_prefix_inplace(s = "aac", "aa"), "c");
    }
    SECTION("空字符串找前缀") {
        std::string s;
        ASSERT_EQ(str::remove_prefix_inplace(s = "", "Hello"), "");
        ASSERT_EQ(str::remove_prefix_inplace(s = "Hello", ""), "Hello");
        ASSERT_EQ(str::remove_prefix_inplace(s = "", ""), "");
    }
    SECTION("字符前缀") {
        std::string s;
        ASSERT_EQ(str::remove_prefix_inplace(s = "HelloWorld", 'H'), "elloWorld");
        ASSERT_EQ(str::remove_prefix_inplace(s = "HelloWorld", 'W'), "HelloWorld");
        ASSERT_EQ(str::remove_prefix_inplace(s = "", 'H'), "");
        ASSERT_EQ(str::remove_prefix_inplace(s = "", '\0'), "");
    }
}

