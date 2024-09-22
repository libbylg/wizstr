#include "catch2/catch_all.hpp"

#include "str.h"
#include "view.h"

TEST_CASE("view::is_blank") {
    REQUIRE(view::is_blank(" \t\t ") == true);
    REQUIRE(view::is_blank("\r\n\v\t ") == false);
    REQUIRE(view::is_blank("") == false);
    REQUIRE(view::is_blank("XYZ#") == false);
    REQUIRE(view::is_blank("\t  XYZ#") == false);
    REQUIRE(view::is_blank("XYZ#\t  ") == false);
}
