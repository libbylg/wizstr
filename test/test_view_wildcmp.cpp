#include "catch2/catch_all.hpp"

#include "str.h"
#include "view.h"

TEST_CASE("view::wildcmp") {
    SECTION("检查是否匹配某个正则表达式") {
        REQUIRE(view::wildcmp("3*", "3bc12def33"));
        REQUIRE(view::wildcmp("3bc12def33", "3bc12def33"));
        REQUIRE(view::wildcmp("3*de?33", "3bc12def33"));
    }
}
