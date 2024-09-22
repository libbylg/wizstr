#include "catch2/catch_all.hpp"

#include "str.h"
#include "view.h"

TEST_CASE("view::is_graph") {
    REQUIRE(view::is_graph("0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz") == true);
    REQUIRE(view::is_graph("!@#$%^&*({[<>]})~`_-+=") == true);
    REQUIRE(view::is_graph("0123\x7F") == false);
    REQUIRE(view::is_graph("0123 ") == false); // 空格比较特殊
    REQUIRE(view::is_graph("") == false);
}
