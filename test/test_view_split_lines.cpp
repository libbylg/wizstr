#include "catch2/catch_all.hpp"

#include "str.h"
#include "view.h"

TEST_CASE("view::split_lines:keep_ends=false") {
    SECTION("一般情况:\\n") {
        std::string_view s{"a\nb\nc"};
        std::vector<std::string_view> expect{
            "a",
            "b",
            "c",
        };
        REQUIRE(view::split_lines(s) == expect);
    }
    SECTION("一般情况:\\r\\n") {
        std::string_view s{"a\r\nb\r\nc"};
        std::vector<std::string_view> expect{
            "a",
            "b",
            "c",
        };
        REQUIRE(view::split_lines(s) == expect);
    }
    SECTION("一般情况:\\r") {
        std::string_view s{"a\rb\rc"};
        std::vector<std::string_view> expect{
            "a",
            "b",
            "c",
        };
        REQUIRE(view::split_lines(s) == expect);
    }
    SECTION("一般情况:\\r收尾") {
        std::string_view s{"a\nb\nc\r"};
        std::vector<std::string_view> expect{
            "a",
            "b",
            "c",
        };
        REQUIRE(view::split_lines(s) == expect);
    }
    SECTION("一般情况:\\n收尾") {
        std::string_view s{"a\nb\nc\n"};
        std::vector<std::string_view> expect{
            "a",
            "b",
            "c",
        };
        REQUIRE(view::split_lines(s) == expect);
    }
    SECTION("一般情况:\\r\\n收尾") {
        std::string_view s{"a\rb\rc\r\n"};
        std::vector<std::string_view> expect{
            "a",
            "b",
            "c",
        };
        REQUIRE(view::split_lines(s) == expect);
    }
    SECTION("连续:\\r") {
        std::string_view s{"a\r\rb\r\rc"};
        std::vector<std::string_view> expect{
            "a",
            "",
            "b",
            "",
            "c",
        };
        std::vector<std::string_view> actual = view::split_lines(s);
        REQUIRE(actual == expect);
    }
    SECTION("连续:\\n") {
        std::string_view s{"a\n\nb\n\nc"};
        std::vector<std::string_view> expect{
            "a",
            "",
            "b",
            "",
            "c",
        };
        REQUIRE(view::split_lines(s) == expect);
    }
    SECTION("单一:\\r") {
        std::string_view s{"\r"};
        std::vector<std::string_view> expect{
            "",
        };
        REQUIRE(view::split_lines(s) == expect);
    }
    SECTION("单一:\\n") {
        std::string_view s{"\n"};
        std::vector<std::string_view> expect{
            "",
        };
        REQUIRE(view::split_lines(s) == expect);
    }
    SECTION("空串") {
        std::string_view s{""};
        std::vector<std::string_view> expect{};
        REQUIRE(view::split_lines(s) == expect);
    }
    SECTION("无任何换行符号") {
        std::string_view s{"abcdef"};
        std::vector<std::string_view> expect{"abcdef"};
        REQUIRE(view::split_lines(s) == expect);
    }
    SECTION("proc:提前结束") {
        std::string_view s{"a\nb\nc\nd\ne\nf"};
        std::vector<std::string_view> expect{"a\n", "b\n"};
        std::vector<std::string_view> result;
        view::split_lines(s, true, [&result](std::string_view item) -> int {
            result.emplace_back(item);
            if (result.size() >= 2) {
                return -1;
            }
            return 0;
        });
        REQUIRE(result == expect);
    }
}

TEST_CASE("view::split_lines:keep_ends=true") {
    SECTION("一般情况:\\n") {
        std::string_view s{"a\nb\nc"};
        std::vector<std::string_view> expect{
            "a\n",
            "b\n",
            "c",
        };
        REQUIRE(view::split_lines(s, true) == expect);
    }
    SECTION("一般情况:\\r\\n") {
        std::string_view s{"a\r\nb\r\nc"};
        std::vector<std::string_view> expect{
            "a\r\n",
            "b\r\n",
            "c",
        };
        REQUIRE(view::split_lines(s, true) == expect);
    }
    SECTION("一般情况:\\r") {
        std::string_view s{"a\rb\rc"};
        std::vector<std::string_view> expect{
            "a\r",
            "b\r",
            "c",
        };
        REQUIRE(view::split_lines(s, true) == expect);
    }
    SECTION("一般情况:\\r收尾") {
        std::string_view s{"a\nb\nc\r"};
        std::vector<std::string_view> expect{
            "a\n",
            "b\n",
            "c\r",
        };
        REQUIRE(view::split_lines(s, true) == expect);
    }
    SECTION("一般情况:\\n收尾") {
        std::string_view s{"a\nb\nc\n"};
        std::vector<std::string_view> expect{
            "a\n",
            "b\n",
            "c\n",
        };
        REQUIRE(view::split_lines(s, true) == expect);
    }
    SECTION("一般情况:\\r\\n收尾") {
        std::string_view s{"a\rb\rc\r\n"};
        std::vector<std::string_view> expect{
            "a\r",
            "b\r",
            "c\r\n",
        };
        REQUIRE(view::split_lines(s, true) == expect);
    }
    SECTION("连续:\\r") {
        std::string_view s{"a\r\rb\r\rc"};
        std::vector<std::string_view> expect{
            "a\r",
            "\r",
            "b\r",
            "\r",
            "c",
        };
        REQUIRE(view::split_lines(s, true) == expect);
    }
    SECTION("连续:\\n") {
        std::string_view s{"a\n\nb\n\nc"};
        std::vector<std::string_view> expect{
            "a\n",
            "\n",
            "b\n",
            "\n",
            "c",
        };
        REQUIRE(view::split_lines(s, true) == expect);
    }
    SECTION("单一:\\r") {
        std::string_view s{"\r"};
        std::vector<std::string_view> expect{
            "\r",
        };
        REQUIRE(view::split_lines(s, true) == expect);
    }
    SECTION("单一:\\n") {
        std::string_view s{"\n"};
        std::vector<std::string_view> expect{
            "\n",
        };
        REQUIRE(view::split_lines(s, true) == expect);
    }
    SECTION("空串") {
        std::string_view s{""};
        std::vector<std::string_view> expect{};
        REQUIRE(view::split_lines(s, true) == expect);
    }
    SECTION("无任何换行符号") {
        std::string_view s{"abcdef"};
        std::vector<std::string_view> expect{"abcdef"};
        REQUIRE(view::split_lines(s, true) == expect);
    }
}