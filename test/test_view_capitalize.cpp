#include "catch2/catch_all.hpp"

#include "str.h"
#include "view.h"

TEST_CASE("view::capitalize") {
    REQUIRE(view::capitalize("abc def") == "Abc def");
    REQUIRE(view::capitalize("Abc def") == "Abc def");
    REQUIRE(view::capitalize("   abc def") == "   abc def");
    REQUIRE(view::capitalize("   Abc def") == "   Abc def");
    REQUIRE(view::capitalize("") == "");
    REQUIRE(view::capitalize("z") == "Z");
    REQUIRE(view::capitalize("Q") == "Q");
    REQUIRE(view::capitalize("123456") == "123456");
}

TEST_CASE("view::is_capitalize") {
    REQUIRE(view::is_capitalize("1") == false);
    REQUIRE(view::is_capitalize("") == false);
    REQUIRE(view::is_capitalize("Abc123DEF") == true);
    REQUIRE(view::is_capitalize("abc123DEF") == false);
    REQUIRE(view::is_capitalize("  A") == false);
    REQUIRE(view::is_capitalize("A") == true);
}
