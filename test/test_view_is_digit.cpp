#include "catch2/catch_all.hpp"

#include "str.h"
#include "view.h"

TEST_CASE("view::is_digit") {
    REQUIRE(view::is_digit("0123456789") == true);
    REQUIRE(view::is_digit("") == false);
    REQUIRE(view::is_digit("123DEF") == false);
    REQUIRE(view::is_digit("123 ") == false);
    REQUIRE(view::is_digit("  123") == false);
    REQUIRE(view::is_digit("ABCDEF") == false);
}
