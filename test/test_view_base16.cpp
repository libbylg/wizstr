#include "catch2/catch_all.hpp"

#include "str.h"
#include "view.h"

TEST_CASE("view::encode_base16") {
    REQUIRE(view::encode_base16("") == "");
    REQUIRE(view::encode_base16("a") == "YQ==");
    REQUIRE(view::encode_base16("ab") == "YWI=");
    REQUIRE(view::encode_base16("abc") == "YWJj");
    REQUIRE(view::encode_base16("abcdef") == "YWJjZGVm");
}


