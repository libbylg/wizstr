#include "catch2/catch_all.hpp"

#include "str.h"
#include "view.h"

TEST_CASE("view::encode_base16") {
    REQUIRE(view::encode_base16("") == "");
    REQUIRE(view::encode_base16("a") == "61");
    REQUIRE(view::encode_base16("ab") == "6162");
    REQUIRE(view::encode_base16("abc") == "616263");
    REQUIRE(view::encode_base16("abcXYZ") == "61626358595A");
}


