#include "catch2/catch_all.hpp"

#include "str.h"
#include "view.h"

template <typename elem1_type>
static auto eq(const std::vector<elem1_type>& a, const std::vector<elem1_type>& b) -> bool {
    if (a.size() != b.size()) {
        return false;
    }

    for (view::size_type i = 0; i < a.size(); i++) {
        if (a[i] != b[i]) {
            return false;
        }
    }

    return true;
}

static auto operator==(const std::vector<std::string_view>& a, const std::vector<std::string_view>& b) -> bool {
    return eq(a, b);
}

TEST_CASE("view::split_lines", "keep_ends=false") {
    SECTION("简单场景") {
        std::string s{"\na\r\nb\ncccc\n"};
        std::vector<std::string_view> expect = {"", "a", "b", "cccc"};

        std::vector<std::string_view> result;
        view::split_lines(s, false, [&result](std::string_view item) -> int {
            result.emplace_back(item);
            return 0;
        });

        REQUIRE(eq(expect, result));
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
    }
}

TEST_CASE("view::split_lines", "keep_ends=true") {
    SECTION("简单场景") {
        std::string s{"\na\r\nb\ncccc\n"};
        std::vector<std::string> expect = {"\n","a\r\n","b\n","cccc\n",""};

        std::vector<std::string> result;
        s.split_lines(true, [&result](std::string::const_pointer p, std::string::size_type n) -> int {
            result.emplace_back(p, n);
            return 0;
        });

        ASSERT_EQ(result, fields);
    }
    SECTION("空串") {
        std::string s{""};
        std::vector<std::string> fields = {
            std::string{""},
        };

        std::vector<std::string> result;
        s.split_lines(true, [&result](std::string::const_pointer p, std::string::size_type n) -> int {
            result.emplace_back(p, n);
            return 0;
        });

        ASSERT_EQ(result, fields);
    }
    SECTION("纯换行") {
        std::string s{"\n"};
        std::vector<std::string> fields = {
            std::string{"\n"},
            std::string{""},
        };

        std::vector<std::string> result;
        s.split_lines(true, [&result](std::string::const_pointer p, std::string::size_type n) -> int {
            result.emplace_back(p, n);
            return 0;
        });

        ASSERT_EQ(result, fields);
    }

    SECTION("综合测试") {
        std::string s{"a\n\ryyy\r\r\nb\n\nc\n\r"};
        std::vector<std::string> fields = {
            std::string{"a\n"},
            std::string{"\r"},
            std::string{"yyy\r"},
            std::string{"\r\n"},
            std::string{"b\n"},
            std::string{"\n"},
            std::string{"c\n"},
            std::string{"\r"},
            std::string{""},
        };

        std::vector<std::string> result;
        s.split_lines(true, [&result](std::string::const_pointer p, std::string::size_type n) -> int {
            result.emplace_back(p, n);
            return 0;
        });

        ASSERT_EQ(result, fields);
    }
}
