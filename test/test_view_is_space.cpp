#include "tester.hpp"

#include "str.hpp"
#include "view.hpp"

TEST_CASE("view::is_space") {
    REQUIRE(view::is_space("\r\n\v\t ") == true);
    REQUIRE(view::is_space("") == false);
    REQUIRE(view::is_space("123DEF") == false);
    REQUIRE(view::is_space("\r\n\v\t #") == false);
}
