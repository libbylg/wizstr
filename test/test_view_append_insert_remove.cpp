#include "catch2/catch_all.hpp"

#include "str.h"
#include "view.h"

TEST_CASE("view::append") {
    SECTION("一般情况") {
        REQUIRE(view::append("aaa", "bbb") == "aaabbb");
    }
    SECTION("空串") {
        REQUIRE(view::append("aaa", "") == "aaa");
        REQUIRE(view::append("", "aaa") == "aaa");
        REQUIRE(view::append("", "") == "");
    }
    SECTION("追加字符") {
        REQUIRE(view::append("aaa", 'A') == "aaaA");
        REQUIRE(view::append("", 'A') == "A");
        REQUIRE(view::append("", '\0') == std::string_view{"\0", 1});
    }
    SECTION("追加重复的字符") {
        REQUIRE(view::append("aaa", 'A', 3) == "aaaAAA");
        REQUIRE(view::append("", 'A', 3) == "AAA");
        REQUIRE(view::append("", '\0', 3) == std::string_view{"\0\0\0", 3});
    }
    SECTION("通过proc提供数据:一般") {
        std::vector<std::string_view> items{
            "Hello",
            "World",
            "",
            "!!!",
        };
        view::size_type index = 0;
        std::string result = view::append("abc", [&items, &index]() -> std::optional<std::string_view> {
            if (index >= items.size()) {
                return std::nullopt;
            }

            return items[index++];
        });
        REQUIRE(result == "abcHelloWorld!!!");
    }
    SECTION("通过proc提供数据:立即终止") {
        std::string result = view::append("abc", []() -> std::optional<std::string_view> {
            return std::nullopt;
        });
        REQUIRE(result == "abc");
    }
}


TEST_CASE("view::prepend") {
    SECTION("一般情况") {
        REQUIRE(view::prepend("aaa", "bbb") == "bbbaaa");
    }
    SECTION("空串") {
        REQUIRE(view::prepend("aaa", "") == "aaa");
        REQUIRE(view::prepend("", "aaa") == "aaa");
        REQUIRE(view::prepend("", "") == "");
    }
    SECTION("追加字符") {
        REQUIRE(view::prepend("aaa", 'A') == "Aaaa");
        REQUIRE(view::prepend("", 'A') == "A");
        REQUIRE(view::prepend("", '\0') == std::string_view{"\0", 1});
    }
    SECTION("追加重复的字符") {
        REQUIRE(view::prepend("aaa", 'A', 3) == "AAAaaa");
        REQUIRE(view::prepend("", 'A', 3) == "AAA");
        REQUIRE(view::prepend("", '\0', 3) == std::string_view{"\0\0\0", 3});
    }
    SECTION("通过proc提供数据:一般") {
        std::vector<std::string_view> items{
            "Hello",
            "World",
            "",
            "!!!",
        };
        view::size_type index = 0;
        std::string result = view::prepend("abc", [&items, &index]() -> std::optional<std::string_view> {
            if (index >= items.size()) {
                return std::nullopt;
            }

            return items[index++];
        });
        REQUIRE(result == "!!!WorldHelloabc");
    }
    SECTION("通过proc提供数据:立即终止") {
        std::string result = view::prepend("abc", []() -> std::optional<std::string_view> {
            return std::nullopt;
        });
        REQUIRE(result == "abc");
    }
}
