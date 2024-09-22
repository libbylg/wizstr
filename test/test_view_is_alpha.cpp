#include "catch2/catch_all.hpp"

#include "str.h"
#include "view.h"

TEST_CASE("view::is_alpha") {
    REQUIRE(view::is_alpha("ABCDEFGHIUKLMNOPQRSTUVWXYZabcdefghiuklmnopqrstuvwxyz") == true);
    REQUIRE(view::is_alpha("!@#$%^&*({[<>]})~`_-+=") == false);
    REQUIRE(view::is_alpha("") == false);
}
