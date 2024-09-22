#include "catch2/catch_all.hpp"

#include "str.h"
#include "view.h"

TEST_CASE("view::is_print") {
    REQUIRE(view::is_print("0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz") == true);
    REQUIRE(view::is_print("!@#$%^&*({[<>]})~`_-+= ") == true);
    REQUIRE(view::is_print("") == false);
    REQUIRE(view::is_print("abc123 \1\2\3\4") == false);
}
