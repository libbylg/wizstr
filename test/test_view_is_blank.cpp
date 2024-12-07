#include "tester.hpp"

#include "str.hpp"
#include "view.hpp"

TEST_CASE("view::is_blank") {
    REQUIRE(view::is_blank(" \t\t ") == true);
    REQUIRE(view::is_blank("\r\n\v\t ") == false);
    REQUIRE(view::is_blank("") == false);
    REQUIRE(view::is_blank("XYZ#") == false);
    REQUIRE(view::is_blank("\t  XYZ#") == false);
    REQUIRE(view::is_blank("XYZ#\t  ") == false);
}
