#include "tester.hpp"

#include "str.hpp"
#include "view.hpp"

TEST_CASE("view::contains") {
    SECTION("简单场景") {
        REQUIRE(view::contains("3bc1233", "3bc") == true);
        REQUIRE(view::contains("3bc1233", "12") == true);
        REQUIRE(view::contains("bc1233", "33") == true);
        REQUIRE(view::contains("bc1233", "31") == false);
        REQUIRE(view::contains("bc1233", "bc1233") == true);
    }
    SECTION("空串") {
        REQUIRE(view::contains("", "") == true);
        REQUIRE(view::contains("abc", "") == true);
        REQUIRE(view::contains("", "abc") == false);
    }
    SECTION("针对字符") {
        REQUIRE(view::contains(std::string_view{"abc\0def", 7}, '\0') == true); // 指定长度时，\0 被包含在内
        REQUIRE(view::contains("abc", '\0') == false);                          // "abc" 转换为 std::string_view 时，并不会包含 \0 字符
        REQUIRE(view::contains("abc", 'a') == true);
        REQUIRE(view::contains("abc", 'b') == true);
        REQUIRE(view::contains("abc", 'c') == true);
    }
}

TEST_CASE("view::count") {
    SECTION("检查是否匹配某个正则表达式") {
        REQUIRE(view::count("3bc12def1233", "12") == 2);
        REQUIRE(view::count("3bc12def1233", "3") == 3);
        REQUIRE(view::count("3bc12def1233", "A") == 0);
        REQUIRE(view::count("3bc12def1233", "def") == 1);
    }
    SECTION("统计子串数量") {
        REQUIRE(view::count("333", "33") == 1);   // 不会考虑重问题
        REQUIRE(view::count("33333", "33") == 2); // 不会考虑重问题
        REQUIRE(view::count("333433343333334", "33") == 5);
        REQUIRE(view::count("333433343333334", "") == 16);
        REQUIRE(view::count("333433343333334", "333") == 4);
    }
    SECTION("统计单字符") {
        REQUIRE(view::count("3bc12def1233", '3') == 3);
        REQUIRE(view::count("3bc12def1233", '\n') == 0);
        REQUIRE(view::count(std::string_view{"3bc12def\0\0#1233", 15}, '\0') == 2);
        REQUIRE(view::count("3bc12def\0\0#1233", '\0') == 0);
    }
    SECTION("空串") {
        REQUIRE(view::count("", "") == 1);
        REQUIRE(view::count("ABCDE", "") == 6);
        REQUIRE(view::count("", "ABCDE") == 0);
    }
    SECTION("指定字符范围") {
        REQUIRE(view::count("3bc12def1233", [](view::value_type ch) -> bool {
            return std::isalpha(ch);
        }) == 5);
        REQUIRE(view::count("3bc12def1233", [](view::value_type ch) -> bool {
            return std::isalnum(ch);
        }) == 12);
        REQUIRE(view::count("3bc12def1233", [](view::value_type ch) -> bool {
            return std::isupper(ch);
        }) == 0);
    }
}