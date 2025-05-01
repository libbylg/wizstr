#include "testing.hpp"

#include "str.hpp"

TEST(test_str, remove_suffix_view) {
    SECTION("一般情况") {
        ASSERT_EQ(str::remove_suffix_view("aaa", "baa"), "aaa");
    }
    SECTION("无共同后缀") {
        ASSERT_EQ(str::remove_suffix_view("aaa", "bbb"), "aaa");
    }
    SECTION("完全相同") {
        ASSERT_EQ(str::remove_suffix_view("aaa", "aaa"), "");
    }
    SECTION("部分为空") {
        ASSERT_EQ(str::remove_suffix_view("", "aaa"), "");
        ASSERT_EQ(str::remove_suffix_view("aaa", ""), "aaa");
    }
    SECTION("空对空") {
        ASSERT_EQ(str::remove_suffix_view("", ""), "");
    }
    SECTION("包含关系") {
        ASSERT_EQ(str::remove_suffix_view("abc", "bc"), "a");
        ASSERT_EQ(str::remove_suffix_view("bc", "abc"), "bc");
    }
    SECTION("字符后缀") {
        ASSERT_EQ(str::remove_suffix_view("HelloWorld", 'd'), "HelloWorl");
        ASSERT_EQ(str::remove_suffix_view("HelloWorld", 'K'), "HelloWorld");
        ASSERT_EQ(str::remove_suffix_view("", 'H'), "");
        ASSERT_EQ(str::remove_suffix_view("", '\0'), "");
    }
}
