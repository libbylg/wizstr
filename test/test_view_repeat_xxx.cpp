#include "catch2/catch_all.hpp"

#include "str.h"
#include "view.h"

TEST_CASE("view::repeat") {
    SECTION("简单字符串重复多次") {
        REQUIRE(view::repeat("0123456789", 5) == "01234567890123456789012345678901234567890123456789");
    }
    SECTION("对空字符串重复：仍然得到的是空字符串") {
        REQUIRE(view::repeat("", 5) == "");
        REQUIRE(view::repeat("", 4096) == "");
    }
    SECTION("对任意字符串重复0次：仍然是空字符串") {
        REQUIRE(view::repeat("", 0) == "");
        REQUIRE(view::repeat("abc", 0) == "");
    }
}
