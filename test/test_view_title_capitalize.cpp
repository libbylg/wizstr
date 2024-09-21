#include "catch2/catch_all.hpp"

#include "str.h"
#include "view.h"

TEST_CASE("view::title") {
    REQUIRE(view::title("abc def") == "Abc Def");
    REQUIRE(view::title("   abc def") == "   Abc Def");
    REQUIRE(view::title("123abc def") == "123Abc Def");
    REQUIRE(view::title("") == "");
    REQUIRE(view::title("z") == "Z");
    REQUIRE(view::title("123456") == "123456");
}

TEST_CASE("view::capitalize") {
    REQUIRE(view::capitalize("abc def") == "Abc def");
    REQUIRE(view::capitalize("   abc def") == "   abc def");
    REQUIRE(view::capitalize("") == "");
    REQUIRE(view::capitalize("z") == "Z");
    REQUIRE(view::capitalize("123456") == "123456");
}

