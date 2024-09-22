#include "catch2/catch_all.hpp"

#include "str.h"
#include "view.h"

TEST_CASE("view::split_words") {
    SECTION("一般情况") {
        REQUIRE(view::split_words("Hello World patppy boy") == std::vector<std::string_view>{"Hello", "World", "patppy", "boy"});
        REQUIRE(view::split_words("Hello\r World\v patppy \t \n boy") == std::vector<std::string_view>{"Hello", "World", "patppy", "boy"});
    }
    SECTION("无任何空白情况") {
        REQUIRE(view::split_words("Hello-World-patppy-boy") == std::vector<std::string_view>{"Hello-World-patppy-boy"});
    }
    SECTION("首尾有空白") {
        REQUIRE(view::split_words("Hello \rWorld\n ") == std::vector<std::string_view>{"Hello", "World"});
        REQUIRE(view::split_words(" \r\v\t\n Hello \rWorld\n ") == std::vector<std::string_view>{"Hello", "World"});
        REQUIRE(view::split_words(" \r\v\t\n Hello \rWorld") == std::vector<std::string_view>{"Hello", "World"});
    }
    SECTION("空串") {
        REQUIRE(view::split_words("") == std::vector<std::string_view>{});
        REQUIRE(view::split_words("", 0) == std::vector<std::string_view>{});
        REQUIRE(view::split_words("", 1) == std::vector<std::string_view>{});
        REQUIRE(view::split_words("", view::npos) == std::vector<std::string_view>{});
    }
    SECTION("全空白串") {
        REQUIRE(view::split_words(" \t\v\r\n ") == std::vector<std::string_view>{});
        REQUIRE(view::split_words(" \t\v\r\n ", 0) == std::vector<std::string_view>{});
        REQUIRE(view::split_words(" \t\v\r\n ", 1) == std::vector<std::string_view>{});
        REQUIRE(view::split_words(" \t\v\r\n ", view::npos) == std::vector<std::string_view>{});
    }
    SECTION("限制数据量") {
        REQUIRE(view::split_words("Hello World patppy boy", 0) == std::vector<std::string_view>{});
        REQUIRE(view::split_words("Hello World patppy boy", 1) == std::vector<std::string_view>{"Hello"});
        REQUIRE(view::split_words("Hello World patppy boy", 2) == std::vector<std::string_view>{"Hello", "World"});
        REQUIRE(view::split_words("Hello World patppy boy", 3) == std::vector<std::string_view>{"Hello", "World", "patppy"});
        REQUIRE(view::split_words("Hello World patppy boy", 4) == std::vector<std::string_view>{"Hello", "World", "patppy", "boy"});
        REQUIRE(view::split_words("Hello World patppy boy", 5) == std::vector<std::string_view>{"Hello", "World", "patppy", "boy"});
        REQUIRE(view::split_words("Hello World patppy boy", view::npos) == std::vector<std::string_view>{"Hello", "World", "patppy", "boy"});
    }
}
