#include "tester.hpp"

#include "str.hpp"
#include "view.hpp"

TEST_CASE("view::is_digit") {
    REQUIRE(view::is_digit("0123456789") == true);
    REQUIRE(view::is_digit("") == false);
    REQUIRE(view::is_digit("123DEF") == false);
    REQUIRE(view::is_digit("123 ") == false);
    REQUIRE(view::is_digit("  123") == false);
    REQUIRE(view::is_digit("ABCDEF") == false);
}
