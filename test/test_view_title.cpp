#include "catch2/catch_all.hpp"

#include "str.h"
#include "view.h"

TEST_CASE("view::title") {
    REQUIRE(view::title("abc def") == "Abc Def");
    REQUIRE(view::title("   abc def  \r\t\n\v 123") == "   Abc Def  \r\t\n\v 123");
    REQUIRE(view::title("123abc def") == "123Abc Def");
    REQUIRE(view::title("") == "");
    REQUIRE(view::title("z") == "Z");
    REQUIRE(view::title("123456") == "123456");
    REQUIRE(view::title("123Abc Def") == "123Abc Def");
}


TEST_CASE("view::is_title") {
    REQUIRE(view::is_title("123456") == false);
    REQUIRE(view::is_title("") == false);
    REQUIRE(view::is_title("Abc 123DEF") == true);
    REQUIRE(view::is_title("Abc 123dEF") == false);
    REQUIRE(view::is_title(" abc 123DEF") == false);
    REQUIRE(view::is_title(" Abc Def 123") == true);
    REQUIRE(view::is_title("  A") == true);
    REQUIRE(view::is_title("A") == true);
}
