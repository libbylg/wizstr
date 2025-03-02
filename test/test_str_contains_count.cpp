#include "tester.hpp"

#include "str.hpp"

TEST(test_str, contains) {
    SECTION("简单场景") {
        ASSERT_EQ(str::contains("3bc1233", "3bc"), true);
        ASSERT_EQ(str::contains("3bc1233", "12"), true);
        ASSERT_EQ(str::contains("bc1233", "33"), true);
        ASSERT_EQ(str::contains("bc1233", "31"), false);
        ASSERT_EQ(str::contains("bc1233", "bc1233"), true);
    }
    SECTION("空串") {
        ASSERT_EQ(str::contains("", ""), true);
        ASSERT_EQ(str::contains("abc", ""), true);
        ASSERT_EQ(str::contains("", "abc"), false);
    }
    SECTION("针对字符") {
        ASSERT_EQ(str::contains(std::string_view{"abc\0def", 7}, '\0'), true); // 指定长度时，\0 被包含在内
        ASSERT_EQ(str::contains("abc", '\0'), false);                          // "abc" 转换为 std::string_view 时，并不会包含 \0 字符
        ASSERT_EQ(str::contains("abc", 'a'), true);
        ASSERT_EQ(str::contains("abc", 'b'), true);
        ASSERT_EQ(str::contains("abc", 'c'), true);
    }
}

TEST(test_str, count) {
    SECTION("检查是否匹配某个正则表达式") {
        ASSERT_EQ(str::count("3bc12def1233", "12"), 2);
        ASSERT_EQ(str::count("3bc12def1233", "3"), 3);
        ASSERT_EQ(str::count("3bc12def1233", "A"), 0);
        ASSERT_EQ(str::count("3bc12def1233", "def"), 1);
    }
    SECTION("统计子串数量") {
        ASSERT_EQ(str::count("333", "33"), 1);   // 不会考虑重问题
        ASSERT_EQ(str::count("33333", "33"), 2); // 不会考虑重问题
        ASSERT_EQ(str::count("333433343333334", "33"), 5);
        ASSERT_EQ(str::count("333433343333334", ""), 16);
        ASSERT_EQ(str::count("333433343333334", "333"), 4);
    }
    SECTION("统计单字符") {
        ASSERT_EQ(str::count("3bc12def1233", '3'), 3);
        ASSERT_EQ(str::count("3bc12def1233", '\n'), 0);
        ASSERT_EQ(str::count(std::string_view{"3bc12def\0\0#1233", 15}, '\0'), 2);
        ASSERT_EQ(str::count("3bc12def\0\0#1233", '\0'), 0);
    }
    SECTION("空串") {
        ASSERT_EQ(str::count("", ""), 1);
        ASSERT_EQ(str::count("ABCDE", ""), 6);
        ASSERT_EQ(str::count("", "ABCDE"), 0);
    }
    SECTION("指定字符范围") {
        ASSERT_EQ(str::count("3bc12def1233", [](str::value_type ch) -> bool {
            return std::isalpha(ch);
        }), 5);
        ASSERT_EQ(str::count("3bc12def1233", [](str::value_type ch) -> bool {
            return std::isalnum(ch);
        }), 12);
        ASSERT_EQ(str::count("3bc12def1233", [](str::value_type ch) -> bool {
            return std::isupper(ch);
        }), 0);
    }
}