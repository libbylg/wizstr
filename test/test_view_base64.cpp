#include "catch2/catch_all.hpp"

#include "str.h"
#include "view.h"

TEST_CASE("view::encode_base64") {
    REQUIRE(view::encode_base64("") == "");
    REQUIRE(view::encode_base64("a") == "YQ==");
    REQUIRE(view::encode_base64("ab") == "YWI=");
    REQUIRE(view::encode_base64("abc") == "YWJj");
    REQUIRE(view::encode_base64("abcdef") == "YWJjZGVm");
}


