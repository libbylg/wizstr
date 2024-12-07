#include "tester.hpp"

#include "str.hpp"
#include "view.hpp"

TEST_CASE("view::encode_base64") {
    REQUIRE(view::encode_base64("") == "");
    REQUIRE(view::encode_base64("a") == "YQ==");
    REQUIRE(view::encode_base64("ab") == "YWI=");
    REQUIRE(view::encode_base64("abc") == "YWJj");
    REQUIRE(view::encode_base64("abcdef") == "YWJjZGVm");
}


