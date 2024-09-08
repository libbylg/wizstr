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
}

//TEST_CASE("view::append_str_n") {
//    SECTION("一般情况") {
//        REQUIRE(view::append("aaa", "bbb", 0) == "aaa");
//        REQUIRE(view::append("aaa", "bbb", 2) == "aaabb");
//        REQUIRE(view::append("aaa", "bbb", 3) == "aaabbb");
//        REQUIRE(view::append("aaa", "bbb", 4) == "aaabbb");
//        REQUIRE(view::append("aaa", "bbb", std::string_view::npos) == "aaabbb");
//    }
//    SECTION("空串") {
//        REQUIRE(view::append("aaa", "", 0) == "aaa");
//        REQUIRE(view::append("aaa", "", 1) == "aaa");
//        REQUIRE(view::append("", "", 0) == "");
//        REQUIRE(view::append("", "", 1) == "");
//    }
//}

