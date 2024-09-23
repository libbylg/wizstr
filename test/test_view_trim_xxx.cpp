#include "catch2/catch_all.hpp"

#include "str.h"
#include "view.h"

TEST_CASE("view::trim_xxx") {
    SECTION("左右都没有空白") {
        REQUIRE(view::trim_left("3bc1233") == "3bc1233");
        REQUIRE(view::trim_right("3bc1233") == "3bc1233");
        REQUIRE(view::trim_surrounding("3bc1233") == "3bc1233");
        REQUIRE(view::trim_anywhere("3bc1233") == "3bc1233");
    }

    SECTION("左边有空白") {
        REQUIRE(view::trim_left("\r  \t\nc1233") == "c1233");
        REQUIRE(view::trim_right("\r  \t\nc1233") == "\r  \t\nc1233");
        REQUIRE(view::trim_surrounding("\r  \t\nc1233") == "c1233");
        REQUIRE(view::trim_anywhere("\r  \t\nc1233") == "c1233");
    }

    SECTION("右边有空白") {
        REQUIRE(view::trim_left("c1233\r  \t\n") == "c1233\r  \t\n");
        REQUIRE(view::trim_right("c1233\r  \t\n") == "c1233");
        REQUIRE(view::trim_surrounding("c1233\r  \t\n") == "c1233");
        REQUIRE(view::trim_anywhere("c1233\r  \t\n") == "c1233");
    }

    SECTION("左右两边都有空白") {
        REQUIRE(view::trim_left("\r  \t\nc1233\r  \t\n") == "c1233\r  \t\n");
        REQUIRE(view::trim_right("\r  \t\nc1233\r  \t\n") == "\r  \t\nc1233");
        REQUIRE(view::trim_surrounding("\r  \t\nc1233\r  \t\n") == "c1233");
        REQUIRE(view::trim_anywhere("\r  \t\nc1233\r  \t\n") == "c1233");
    }

    SECTION("中间有空白") {
        REQUIRE(view::trim_left("\r  \t\nc12\r  \t\n33\r  \t\n") == "c12\r  \t\n33\r  \t\n");
        REQUIRE(view::trim_right("\r  \t\nc12\r  \t\n33\r  \t\n") == "\r  \t\nc12\r  \t\n33");
        REQUIRE(view::trim_surrounding("\r  \t\nc12\r  \t\n33\r  \t\n") == "c12\r  \t\n33");
        REQUIRE(view::trim_anywhere("\r  \t\nc12\r  \t\n33\r  \t\n") == "c1233");
    }

    SECTION("空串") {
        REQUIRE(view::trim_left("") == "");
        REQUIRE(view::trim_right("") == "");
        REQUIRE(view::trim_surrounding("") == "");
        REQUIRE(view::trim_anywhere("") == "");
    }

    SECTION("全空白串") {
        REQUIRE(view::trim_left("\r\n \t") == "");
        REQUIRE(view::trim_right("\r\n \t") == "");
        REQUIRE(view::trim_surrounding("\r\n \t") == "");
        REQUIRE(view::trim_anywhere("\r\n \t") == "");
    }

    SECTION("指定字符") {
        REQUIRE(view::trim_left("33c1233", [](std::string::value_type ch) -> bool {
            return ch == '3';
        }) == "c1233");
        REQUIRE(view::trim_right("33c1233", [](std::string::value_type ch) -> bool {
            return ch == '3';
        }) == "33c12");
        REQUIRE(view::trim_surrounding("33c1233", [](std::string::value_type ch) -> bool {
            return ch == '3';
        }) == "c12");
        REQUIRE(view::trim_anywhere("33c1233", [](std::string::value_type ch) -> bool {
            return ch == '3';
        }) == "c12");
    }
}
