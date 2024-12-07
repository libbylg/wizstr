#include "tester.hpp"

#include "str.hpp"
#include "view.hpp"

TEST_CASE("view::swap_case") {
    SECTION("一般情况") {
        REQUIRE(view::swap_case("  AabbCC中华人民共和国") == "  aABBcc中华人民共和国");
        REQUIRE(view::swap_case("中华人民共和国") == "中华人民共和国");
        REQUIRE(view::swap_case("0123456789*&^%#$@") == "0123456789*&^%#$@");
    }
    SECTION("空串") {
        REQUIRE(view::swap_case("") == "");
    }
    SECTION("全部是大写或者小写") {
        REQUIRE(view::swap_case("ABCDEFGHIJK") == "abcdefghijk");
        REQUIRE(view::swap_case("abcdefghijk") == "ABCDEFGHIJK");
    }
}

TEST_CASE("view::to_upper") {
    SECTION("一般情况") {
        REQUIRE(view::to_upper("  AabbCC中华人民共和国") == "  AABBCC中华人民共和国");
        REQUIRE(view::to_upper("中华人民共和国") == "中华人民共和国");
        REQUIRE(view::to_upper("0123456789*&^%#$@") == "0123456789*&^%#$@");
    }
    SECTION("空串") {
        REQUIRE(view::to_upper("") == "");
    }
    SECTION("全部是大写或者小写") {
        REQUIRE(view::to_upper("ABCDEFGHIJK") == "ABCDEFGHIJK");
        REQUIRE(view::to_upper("abcdefghijk") == "ABCDEFGHIJK");
    }
}

TEST_CASE("view::to_lower") {
    SECTION("一般情况") {
        REQUIRE(view::to_lower("  AabbCC中华人民共和国") == "  aabbcc中华人民共和国");
        REQUIRE(view::to_lower("中华人民共和国") == "中华人民共和国");
        REQUIRE(view::to_lower("0123456789*&^%#$@") == "0123456789*&^%#$@");
    }
    SECTION("空串") {
        REQUIRE(view::to_lower("") == "");
    }
    SECTION("全部是大写或者小写") {
        REQUIRE(view::to_lower("ABCDEFGHIJK") == "abcdefghijk");
        REQUIRE(view::to_lower("abcdefghijk") == "abcdefghijk");
    }
}