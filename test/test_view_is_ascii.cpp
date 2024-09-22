#include "catch2/catch_all.hpp"

#include "str.h"
#include "view.h"

TEST_CASE("view::is_ascii") {
    REQUIRE(view::is_ascii("0123456789ABCDEF#$%\r\n\t\v ") == true);
    REQUIRE(view::is_ascii("ABCDEF") == true);
    REQUIRE(view::is_ascii("123DEF") == true);
}
