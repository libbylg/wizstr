#include "tester.hpp"

#include "str.hpp"
#include "view.hpp"

TEST_CASE("view::is_alnum") {
    REQUIRE(view::is_alnum("0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz") == true);
    REQUIRE(view::is_alnum("!@#$%^&*({[<>]})~`_-+=") == false);
    REQUIRE(view::is_alnum("") == false);
}
