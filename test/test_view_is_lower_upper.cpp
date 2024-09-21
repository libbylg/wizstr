#include "catch2/catch_all.hpp"

#include "str.h"
#include "view.h"

TEST_CASE("view::is_lower") {
    SECTION("全小写") {
        REQUIRE(view::is_lower("abcdef") == true);
    }
    SECTION("只有一个大写") {
        REQUIRE(view::is_lower("abcdeF") == false);
    }
    SECTION("无字母") {
        REQUIRE(view::is_lower("092320914#$@#$@35") == false);
    }
    SECTION("混合") {
        REQUIRE(view::is_lower("abcDEF0923") == false);
    }
    SECTION("空串") {
        REQUIRE(view::is_lower("") == false);
    }
}

TEST_CASE("view::is_upper") {
    SECTION("全大写") {
        REQUIRE(view::is_upper("ABCDEF") == true);
    }
    SECTION("只有一个小写") {
        REQUIRE(view::is_upper("ABCDEf") == false);
    }
    SECTION("无字母") {
        REQUIRE(view::is_upper("092320914#$@#$@35") == false);
    }
    SECTION("混合") {
        REQUIRE(view::is_upper("abcDEF0923") == false);
    }
    SECTION("空串") {
        REQUIRE(view::is_upper("") == false);
    }
}