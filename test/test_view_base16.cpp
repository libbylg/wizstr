#include "tester.hpp"

#include "str.hpp"
#include "view.hpp"

TEST_CASE("view::encode_base16") {
    REQUIRE(view::encode_base16("") == "");
    REQUIRE(view::encode_base16("a") == "61");
    REQUIRE(view::encode_base16("ab") == "6162");
    REQUIRE(view::encode_base16("abc") == "616263");
    REQUIRE(view::encode_base16("abcXYZ", true) == "61626358595A");
}


