#include "catch2/catch_all.hpp"

#include "str.h"
#include "view.h"

TEST_CASE("view::find_next_word") {
    SECTION("不同起始位置") {
        REQUIRE(view::find_next_word("abc def", 0) == std::string("abc"));
        REQUIRE(view::find_next_word("abc def", 1) == std::string("bc"));
        REQUIRE(view::find_next_word("abc def", 2) == std::string("c"));
        REQUIRE(view::find_next_word("abc def", 3) == std::string("def"));
        REQUIRE(view::find_next_word("abc def", 4) == std::string("def"));
        REQUIRE(view::find_next_word("abc def", 5) == std::string("ef"));
        REQUIRE(view::find_next_word("abc def", 6) == std::string("f"));
        REQUIRE(view::find_next_word("abc def", 7) == std::string(""));
        REQUIRE(view::find_next_word("abc def", view::npos) == std::string(""));
    }
    SECTION("不同空格和多段字符串组合") {
        REQUIRE(view::find_next_word(" \r\t\n abc \r\t\n def \r\t\n ", 0) == std::string("abc"));
        REQUIRE(view::find_next_word(" \r\t\n abc \r\t\n def \r\t\n ", 8) == std::string("def"));
        REQUIRE(view::find_next_word(" \r\t\n abc \r\t\n def \r\t\n ", 16) == std::string(""));
    }
    SECTION("无任何空白的串") {
        REQUIRE(view::find_next_word("abc", 0) == std::string("abc"));
        REQUIRE(view::find_next_word("abcdef", view::npos) == std::string(""));
    }
    SECTION("空串") {
        REQUIRE(view::find_next_word("", 0) == std::string(""));
        REQUIRE(view::find_next_word("", view::npos) == std::string(""));
    }
    SECTION("全空白串") {
        REQUIRE(view::find_next_word(" \r\t\n ", 1) == std::string(""));
    }
}

TEST_CASE("view::iter_next_word") {
    SECTION("不同起始位置") {
        view::size_type pos = 0;
        REQUIRE(view::iter_next_word("abc def", pos) == std::string("abc"));
        REQUIRE(pos == 3);
        REQUIRE(view::iter_next_word("abc def", pos) == std::string("def"));
        REQUIRE(pos == 7);
        REQUIRE(view::iter_next_word("abc def", pos) == std::string(""));
        REQUIRE(pos == 7);

        pos == view::npos;
        REQUIRE(view::iter_next_word("abc def", pos) == std::string(""));
        REQUIRE(pos == 7);
        REQUIRE(view::iter_next_word("abc def", pos) == std::string(""));
        REQUIRE(pos == 7);
    }
    SECTION("不同空格和多段字符串组合") {
        view::size_type pos = 0;
        REQUIRE(view::iter_next_word(" \r\t\n abc \r\t\n def \r\t\n ", pos) == std::string("abc"));
        REQUIRE(pos == 8);
        REQUIRE(view::iter_next_word(" \r\t\n abc \r\t\n def \r\t\n ", pos) == std::string("def"));
        REQUIRE(pos == 16);
        REQUIRE(view::iter_next_word(" \r\t\n abc \r\t\n def \r\t\n ", pos) == std::string(""));
        REQUIRE(pos == 21);
    }
    SECTION("无任何空白的串") {
        view::size_type pos = 0;
        REQUIRE(view::iter_next_word("abc", pos) == std::string("abc"));
        REQUIRE(pos == 3);

        pos = view::npos;
        REQUIRE(view::iter_next_word("abcdef", pos) == std::string(""));
        REQUIRE(pos == 6);
    }
    SECTION("空串") {
        view::size_type pos = 0;
        REQUIRE(view::iter_next_word("", pos) == std::string(""));
        REQUIRE(pos == 0);

        pos = view::npos;
        REQUIRE(view::iter_next_word("", pos) == std::string(""));
        REQUIRE(pos == 0);
    }
    SECTION("全空白串") {
        view::size_type pos = 0;
        REQUIRE(view::iter_next_word(" \r\t\n ", pos) == std::string(""));
        REQUIRE(pos == 5);
    }
}