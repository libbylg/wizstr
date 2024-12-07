#include "tester.hpp"

#include "str.hpp"
#include "view.hpp"

TEST_CASE("view::is_alpha") {
    REQUIRE(view::is_alpha("ABCDEFGHIUKLMNOPQRSTUVWXYZabcdefghiuklmnopqrstuvwxyz") == true);
    REQUIRE(view::is_alpha("!@#$%^&*({[<>]})~`_-+=") == false);
    REQUIRE(view::is_alpha("") == false);
}
