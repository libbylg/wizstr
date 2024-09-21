#include "catch2/catch_all.hpp"

#include "str.h"
#include "view.h"

#include <array>

TEST_CASE("view::foreach_word") {
    SECTION("一般场景:1") {
        std::string result;
        view::foreach_word("\r\n\t A\r\n\t B\r\n\t C\r\n\t ", [&result](std::string_view word) -> int {
            result.append(word);
            return 0;
        });
        REQUIRE(result == "ABC");
    }
    SECTION("一般场景:2") {
        std::string result;
        view::foreach_word("DEF", [&result](std::string_view word) -> int {
            result.append(word);
            return 0;
        });
        REQUIRE(result == "DEF");
    }
    SECTION("就只有一个单词") {
        std::string result;
        view::foreach_word("DEF", [&result](std::string_view word) -> int {
            result.append(word);
            return 0;
        });
        REQUIRE(result == "DEF");
    }
    SECTION("空串:1") {
        std::string result;
        view::foreach_word("", [&result](std::string_view word) -> int {
            result.append(word);
            return 0;
        });
        REQUIRE(result == "");
    }
    SECTION("空串:2") {
        std::string result;
        view::foreach_word("", 1, [&result](std::string_view word) -> int {
            result.append(word);
            return 0;
        });
        REQUIRE(result == "");
    }
    SECTION("全空白") {
        std::string result;
        view::foreach_word("\r\n\t \n\t ", 1, [&result](std::string_view word) -> int {
            result.append(word);
            return 0;
        });
        REQUIRE(result == "");
    }
    SECTION("使用pos:正常情况") {
        std::string result;
        view::foreach_word("   ABC   DEF GHI", 6, [&result](std::string_view word) -> int {
            result.append(word);
            return 0;
        });
        REQUIRE(result == "DEFGHI");
    }
    SECTION("使用pos:指定pos超出范围") {
        std::string result;
        view::foreach_word("   ABC   DEF GHI", 16, [&result](std::string_view word) -> int {
            result.append(word);
            return 0;
        });
        REQUIRE(result == "");
    }
    SECTION("使用pos:npos") {
        std::string result;
        view::foreach_word("   ABC   DEF GHI", view::npos, [&result](std::string_view word) -> int {
            result.append(word);
            return 0;
        });
        REQUIRE(result == "");
    }
    SECTION("返回pos和n:1") {
        using elem_type = std::tuple<view::size_type, view::size_type>;
        std::vector<elem_type> result;
        view::foreach_word("   ABC   DEF GHXWYI", [&result](view::size_type pos, view::size_type n) -> int {
            result.emplace_back(pos, n);
            return 0;
        });
        REQUIRE(result.size() == 3);
        REQUIRE(result[0] == elem_type{3, 3});
        REQUIRE(result[1] == elem_type{9, 3});
        REQUIRE(result[2] == elem_type{13, 6});
    }
    SECTION("返回pos和n:2") {
        using elem_type = std::tuple<view::size_type, view::size_type>;
        std::vector<elem_type> result;
        view::foreach_word("   ABC   DEF GHXWYI", 4, [&result](view::size_type pos, view::size_type n) -> int {
            result.emplace_back(pos, n);
            return 0;
        });
        REQUIRE(result.size() == 3);
        REQUIRE(result[0] == elem_type{4, 2});
        REQUIRE(result[1] == elem_type{9, 3});
        REQUIRE(result[2] == elem_type{13, 6});
    }
    SECTION("返回提前结束") {
        using elem_type = std::tuple<view::size_type, view::size_type>;
        std::vector<elem_type> result;
        view::foreach_word("   ABC   DEF GHXWYI", 4, [&result](view::size_type pos, view::size_type n) -> int {
            result.emplace_back(pos, n);
            if ((pos + n) > 10) {
                return -1;
            }
            return 0;
        });
        REQUIRE(result.size() == 2);
        REQUIRE(result[0] == elem_type{4, 2});
        REQUIRE(result[1] == elem_type{9, 3});
    }
}