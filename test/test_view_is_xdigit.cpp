#include "catch2/catch_all.hpp"

#include "str.h"
#include "view.h"

TEST_CASE("view::is_xdigit") {
    REQUIRE(view::is_xdigit("0123456789ABCDEF") == true);
    REQUIRE(view::is_xdigit("ABCDEF") == true);
    REQUIRE(view::is_xdigit("123DEF") == true);

    REQUIRE(view::is_xdigit("0123456789abcdefABCDEF") == true);
    REQUIRE(view::is_xdigit("abcdefABCDEF") == true);
    REQUIRE(view::is_xdigit("123deF") == true);

    REQUIRE(view::is_xdigit("ABCDEFGHIJ") == false);
    REQUIRE(view::is_xdigit("abcDEFghij") == false);
    REQUIRE(view::is_xdigit("123GHI") == false);
    REQUIRE(view::is_xdigit("") == false);
    REQUIRE(view::is_xdigit("123 ") == false);
    REQUIRE(view::is_xdigit("  123") == false);
}
