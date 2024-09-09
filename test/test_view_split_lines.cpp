#include "catch2/catch_all.hpp"

#include "str.h"
#include "view.h"
#include "test-compares.h"

TEST_CASE("view::split_lines:keep_ends=false") {
    SECTION("简单场景") {
        std::string s{"\na\r\nb\ncccc\n"};
        std::vector<std::string_view> expect = {"", "a", "b", "cccc"};

        std::vector<std::string_view> result;
        view::split_lines(s, false, [&result](std::string_view item) -> int {
            result.emplace_back(item);
            return 0;
        });

        REQUIRE(result == expect);
        REQUIRE(view::split_lines(s, false) == expect);
    }
    SECTION("空串") {
        std::string s{""};
        std::vector<std::string_view> expect = {};

        std::vector<std::string_view> result;
        view::split_lines(s, false, [&result](std::string_view item) -> int {
            result.emplace_back(item);
            return 0;
        });

        REQUIRE(expect == result);
        REQUIRE(view::split_lines(s, false) == expect);
    }
    SECTION("各种换行场景") {
        std::string s{"\n\r\r\n\r"};
        std::vector<std::string_view> expect = {"", "", "", ""};

        std::vector<std::string_view> result;
        view::split_lines(s, false, [&result](std::string_view item) -> int {
            result.emplace_back(item);
            return 0;
        });

        REQUIRE(result == expect);
        REQUIRE(view::split_lines(s, false) == expect);
    }
    SECTION("综合测试") {
        std::string s{"a\n\ryyy\r\r\nb\n\nc\n\r"};
        std::vector<std::string_view> expect = {"a", "", "yyy", "", "b", "", "c", ""};

        std::vector<std::string_view> result;
        view::split_lines(s, false, [&result](std::string_view item) -> int {
            result.emplace_back(item);
            return 0;
        });

        REQUIRE(result == expect);
        REQUIRE(view::split_lines(s, false) == expect);
    }
}

TEST_CASE("view::split_lines:keep_ends=true") {
    SECTION("简单场景") {
        std::string s{"\na\r\nb\ncccc\n"};
        std::vector<std::string_view> expect = {"\n", "a\r\n", "b\n", "cccc\n"};

        std::vector<std::string_view> result;
        view::split_lines(s, true, [&result](std::string_view item) -> int {
            result.emplace_back(item);
            return 0;
        });

        REQUIRE(result == expect);
        REQUIRE(view::split_lines(s, true) == expect);
    }
    SECTION("空串") {
        std::string s{""};
        std::vector<std::string_view> expect = {};

        std::vector<std::string_view> result;
        view::split_lines(s, true, [&result](std::string_view item) -> int {
            result.emplace_back(item);
            return 0;
        });

        REQUIRE(result == expect);
        REQUIRE(view::split_lines(s, true) == expect);
    }
    SECTION("纯换行") {
        std::string s{"\n"};
        std::vector<std::string_view> expect = {"\n"};

        std::vector<std::string_view> result;
        view::split_lines(s, true, [&result](std::string_view item) -> int {
            result.emplace_back(item);
            return 0;
        });

        REQUIRE(result == expect);
        REQUIRE(view::split_lines(s, true) == expect);
    }

    SECTION("综合测试") {
        std::string s{"a\n\ryyy\r\r\nb\n\nc\n\r"};
        std::vector<std::string_view> expect = {"a\n", "\r", "yyy\r", "\r\n", "b\n", "\n", "c\n", "\r"};

        std::vector<std::string_view> result;
        view::split_lines(s, true, [&result](std::string_view item) -> int {
            result.emplace_back(item);
            return 0;
        });

        REQUIRE(result == expect);
        REQUIRE(view::split_lines(s, true) == expect);
    }
    SECTION("最后一行没有换行符号") {
        std::string s{"a\nyyy\nb\n\ncdef"};
        std::vector<std::string_view> expect = {"a\n", "yyy\n", "b\n", "\n", "cdef"};

        std::vector<std::string_view> result;
        view::split_lines(s, true, [&result](std::string_view item) -> int {
            result.emplace_back(item);
            return 0;
        });

        REQUIRE(result == expect);
        REQUIRE(view::split_lines(s, true) == expect);
    }
    SECTION("单行无换行符") {
        std::string s{"ayyynbncdef"};
        std::vector<std::string_view> expect = {"ayyynbncdef"};

        std::vector<std::string_view> result;
        view::split_lines(s, true, [&result](std::string_view item) -> int {
            result.emplace_back(item);
            return 0;
        });

        REQUIRE(result == expect);
        REQUIRE(view::split_lines(s, true) == expect);
    }
    SECTION("提前终止") {
        std::string s{"a\nyyy\nb\n\ncdef"};
        std::vector<std::string_view> expect = {"a\n", "yyy\n", "b\n"};

        std::vector<std::string_view> result;
        view::split_lines(s, true, [&result](std::string_view item) -> int {
            result.emplace_back(item);
            return (result.size() < 3 ? 0 : -1);
        });

        REQUIRE(result == expect);
        REQUIRE(view::split_lines(s, true) == std::vector<std::string_view>{"a\n", "yyy\n", "b\n", "\n", "cdef"});
    }
}
