#include "catch2/catch_all.hpp"

#include "str.h"
#include "view.h"

TEST_CASE("view::space") {
    SECTION("简单字符串重复多次") {
        REQUIRE(view::spaces(5) == "     ");
        REQUIRE(view::spaces(0) == "");
    }
}

TEST_CASE("view::skip_space_remain") {
    REQUIRE(view::skip_space_remain("abc") == "abc");
    REQUIRE(view::skip_space_remain("   abc  ") == "abc  ");
    REQUIRE(view::skip_space_remain("   abc  ", 1) == "abc  ");
    REQUIRE(view::skip_space_remain("   abc  ", 2) == "abc  ");
    REQUIRE(view::skip_space_remain("   abc  ", 3) == "abc  ");
    REQUIRE(view::skip_space_remain("   abc  ", 10) == "");
    REQUIRE(view::skip_space_remain(" \r\t\n ", 0) == "");
    REQUIRE(view::skip_space_remain("") == "");
    REQUIRE(view::skip_space_remain("abc  def") == "abc  def");
}

TEST_CASE("view::skip_space") {
    REQUIRE(view::skip_space("abc", 0) == 0);
    REQUIRE(view::skip_space("   abc  ", 0) == 3);
    REQUIRE(view::skip_space(" \r\t\n ", 0) == 5);
    REQUIRE(view::skip_space("", 0) == 0);
    REQUIRE(view::skip_space("abc  def", 7) == 7);
    REQUIRE(view::skip_space("abc  def", 8) == 8);
    REQUIRE(view::skip_space("abc  def", view::npos) == 8);
}
