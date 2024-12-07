#include "tester.hpp"

#include "str.hpp"
#include "view.hpp"

TEST_CASE("view::is_identifier") {
    REQUIRE(view::is_identifier("0123456789") == false);
    REQUIRE(view::is_identifier("") == false);
    REQUIRE(view::is_identifier("123DEF") == false);
    REQUIRE(view::is_identifier("ABC ") == false);
    REQUIRE(view::is_identifier("  ABC") == false);
    REQUIRE(view::is_identifier("ABCDEF") == true);
    REQUIRE(view::is_identifier("_") == true);
    REQUIRE(view::is_identifier("_123") == true);
    REQUIRE(view::is_identifier("_ABC") == true);
    REQUIRE(view::is_identifier("XXX_HOME") == true);
}
