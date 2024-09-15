#include "catch2/catch_all.hpp"

#include "str.h"
#include "view.h"

TEST_CASE("view::invert:all") {
    SECTION("反转偶数长度的字符串") {
        REQUIRE(view::invert("0123456789") == "9876543210");
    }
    SECTION("反转奇数长度的字符串") {
        REQUIRE(view::invert("1234567") == "7654321");
    }
    SECTION("反转空字符串") {
        REQUIRE(view::invert("") == "");
    }
    SECTION("反转单字符串") {
        REQUIRE(view::invert("\n") == "\n");
    }
}


TEST_CASE("view::invert:range") {
    SECTION("反转范围在字符串内部:起点和终点都有效") {
        REQUIRE(view::invert("0123456789", 3, 5) == "0127654389");
        REQUIRE(view::invert("0123456789", 3, 7) == "0129876543");
    }
    SECTION("反转范围在字符串外:起始点超出字符串右边界") {
        REQUIRE(view::invert("1234567", 7, 2) == "1234567");
        REQUIRE(view::invert("1234567", 10, 2) == "1234567");
        REQUIRE(view::invert("1234567", view::npos, 2) == "1234567");
    }
    SECTION("反转空字符串") {
        REQUIRE(view::invert("", 0, 1) == "");
        REQUIRE(view::invert("", 1, 100) == "");
    }
    SECTION("指定反转串的数量太大，超出原串长度") {
        REQUIRE(view::invert("0123456789", 3, 7) == "0129876543");
        REQUIRE(view::invert("0123456789", 3, 100) == "0129876543");
        REQUIRE(view::invert("0123456789", 3, view::npos) == "0129876543");
    }
    SECTION("反转串的长度为0或者1：无反转效果") {
        REQUIRE(view::invert("0123456789", 3, 0) == "0123456789");
        REQUIRE(view::invert("0123456789", 3, 1) == "0123456789");
        REQUIRE(view::invert("0123456789", 20, 0) == "0123456789");
        REQUIRE(view::invert("0123456789", 20, 1) == "0123456789");
        REQUIRE(view::invert("0123456789", view::npos, 0) == "0123456789");
        REQUIRE(view::invert("0123456789", view::npos, 1) == "0123456789");
        REQUIRE(view::invert("", 0, 0) == "");
        REQUIRE(view::invert("", 0, 1) == "");
        REQUIRE(view::invert("", view::npos, 0) == "");
        REQUIRE(view::invert("", view::npos, 1) == "");
        REQUIRE(view::invert("C", 0, 0) == "C");
        REQUIRE(view::invert("C", 0, 1) == "C");
        REQUIRE(view::invert("C", view::npos, 0) == "C");
        REQUIRE(view::invert("C", view::npos, 1) == "C");
    }
}