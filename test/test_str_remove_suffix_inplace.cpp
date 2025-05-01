#include "testing.hpp"

#include "str.hpp"

TEST(test_str, remove_suffix_inplace) {
    SECTION("一般情况") {
        std::string s;
        ASSERT_EQ(str::remove_suffix_inplace(s = "aaa", "baa"), "aaa");
    }
    SECTION("无共同后缀") {
        std::string s;
        ASSERT_EQ(str::remove_suffix_inplace(s = "aaa", "bbb"), "aaa");
    }
    SECTION("完全相同") {
        std::string s;
        ASSERT_EQ(str::remove_suffix_inplace(s = "aaa", "aaa"), "");
    }
    SECTION("部分为空") {
        std::string s;
        ASSERT_EQ(str::remove_suffix_inplace(s = "", "aaa"), "");
        ASSERT_EQ(str::remove_suffix_inplace(s = "aaa", ""), "aaa");
    }
    SECTION("空对空") {
        std::string s;
        ASSERT_EQ(str::remove_suffix_inplace(s = "", ""), "");
    }
    SECTION("包含关系") {
        std::string s;
        ASSERT_EQ(str::remove_suffix_inplace(s = "abc", "bc"), "a");
        ASSERT_EQ(str::remove_suffix_inplace(s = "bc", "abc"), "bc");
    }
    SECTION("字符后缀") {
        std::string s;
        ASSERT_EQ(str::remove_suffix_inplace(s = "HelloWorld", 'd'), "HelloWorl");
        ASSERT_EQ(str::remove_suffix_inplace(s = "HelloWorld", 'K'), "HelloWorld");
        ASSERT_EQ(str::remove_suffix_inplace(s = "", 'H'), "");
        ASSERT_EQ(str::remove_suffix_inplace(s = "", '\0'), "");
    }
}
