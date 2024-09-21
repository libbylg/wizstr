#include "catch2/catch_all.hpp"

#include "str.h"
#include "view.h"

TEST_CASE("view::icompare") {
    SECTION("同长字符串对比") {
        REQUIRE(view::icompare("ABCdef", "abcdef") == 0);
        REQUIRE(view::icompare("ABCDEF", "ABCDEF") == 0);
        REQUIRE(view::icompare("abcdef", "ABCDEF") == 0);
        REQUIRE(view::icompare("01234$%#$%#$^", "01234$%#$%#$^") == 0);
        REQUIRE(view::icompare("abcdef", "ABC123") > 0);
    }
    SECTION("长短不一的对比") {
        REQUIRE(view::icompare("abc", "ABCdef") < 0);
        REQUIRE(view::icompare("ABCdef", "abc") > 0);
    }
    SECTION("空串") {
        REQUIRE(view::icompare("", "") == 0);
        REQUIRE(view::icompare("", "abc") < 0);
        REQUIRE(view::icompare("abc", "") > 0);
    }
    SECTION("指定最大对比长度") {
        REQUIRE(view::icompare("", "", 0) == 0);
        REQUIRE(view::icompare("", "abc", 1) < 0);
        REQUIRE(view::icompare("abc", "", 1) > 0);
        REQUIRE(view::icompare("abc", "", 0) == 0);
        REQUIRE(view::icompare("abc", "ABCdef", 3) == 0);
        REQUIRE(view::icompare("ABCdef", "abc", 4) > 0);
        REQUIRE(view::icompare("ABCdef", "abcDEF", 6) == 0);
        REQUIRE(view::icompare("ABCdef", "abcDEF", 7) == 0);
        REQUIRE(view::icompare("ABCdef", "abcDEF", view::npos) == 0);
    }
}

TEST_CASE("view::iequals") {
    SECTION("同长字符串对比") {
        REQUIRE(view::iequals("ABCdef", "abcdef") == true);
        REQUIRE(view::iequals("ABCDEF", "ABCDEF") == true);
        REQUIRE(view::iequals("abcdef", "ABCDEF") == true);
        REQUIRE(view::iequals("01234$%#$%#$^", "01234$%#$%#$^") == true);
        REQUIRE(view::iequals("abcdef", "ABC123") == false);
    }
    SECTION("长短不一的对比") {
        REQUIRE(view::iequals("abc", "ABCdef") == false);
        REQUIRE(view::iequals("ABCdef", "abc") == false);
    }
    SECTION("空串") {
        REQUIRE(view::iequals("", "") == true);
        REQUIRE(view::iequals("", "abc") == false);
        REQUIRE(view::iequals("abc", "") == false);
    }
    SECTION("指定最大对比长度") {
        REQUIRE(view::iequals("", "", 0) == true);
        REQUIRE(view::iequals("", "abc", 1) == false);
        REQUIRE(view::iequals("abc", "", 1) == false);
        REQUIRE(view::iequals("abc", "abc", 0) == true);
        REQUIRE(view::iequals("abc", "ABCdef", 3) == true);
        REQUIRE(view::iequals("ABCdef", "abc", 4) == false);
        REQUIRE(view::iequals("ABCdef", "abcDEF", 6) == true);
        REQUIRE(view::iequals("ABCdef", "abcDEF", 7) == true);
        REQUIRE(view::iequals("ABCdef", "abcDEF", view::npos) == true);
    }
}