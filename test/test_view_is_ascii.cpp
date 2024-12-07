#include "tester.hpp"

#include "str.hpp"
#include "view.hpp"

TEST_CASE("view::is_ascii") {
    REQUIRE(view::is_ascii("0123456789ABCDEF#$%\r\n\t\v ") == true);
    REQUIRE(view::is_ascii("ABCDEF") == true);
    REQUIRE(view::is_ascii("123DEF") == true);
    REQUIRE(view::is_ascii(std::string_view{"\0\0", 2}) == true);
    REQUIRE(view::is_ascii("123你好DEF") == false);
    REQUIRE(view::is_ascii("") == false);
}
