#include "catch2/catch_all.hpp"

#include "str.h"
#include "view.h"

TEST_CASE("view::is_space") {
    REQUIRE(view::is_space("\r\n\v\t ") == true);
    REQUIRE(view::is_space("") == false);
    REQUIRE(view::is_space("123DEF") == false);
    REQUIRE(view::is_space("\r\n\v\t #") == false);
}
