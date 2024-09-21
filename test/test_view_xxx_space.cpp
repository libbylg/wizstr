#include "catch2/catch_all.hpp"

#include "str.h"
#include "view.h"

TEST_CASE("view::space") {
    SECTION("简单字符串重复多次") {
        REQUIRE(view::spaces(5) == "     ");
        REQUIRE(view::spaces(0) == "");
    }
}

TEST_CASE("view::skip_space") {
    REQUIRE(view::skip_space("abc") == "abc");
    REQUIRE(view::skip_space("   abc  ") == "abc  ");
    REQUIRE(view::skip_space(" \r\t\n ") == "");
    REQUIRE(view::skip_space("") == "");
    REQUIRE(view::skip_space("abc  def", 8) == "");
    REQUIRE(view::skip_space("abc  def", view::npos) == "");
}
