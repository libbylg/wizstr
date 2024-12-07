#include "tester.h"

#include "str.hpp"
#include "view.hpp"

TEST_CASE("view::wildcmp") {
    SECTION("检查是否匹配某个正则表达式") {
        REQUIRE(view::wildcmp("3*", "3bc12def33"));
        REQUIRE(view::wildcmp("3abc", "3abc"));
        REQUIRE(view::wildcmp("?", "3"));
        REQUIRE(view::wildcmp("????", "3abc"));
        REQUIRE(view::wildcmp("", "3abc") == false);
        REQUIRE(view::wildcmp("", ""));
        REQUIRE(view::wildcmp("*3", "3bc12def33"));
        REQUIRE(view::wildcmp("*3?", "3bc12def33"));
        REQUIRE(view::wildcmp("*?3", "3bc12def33"));
        REQUIRE(view::wildcmp("3bc12def33", "3bc12def33"));
        REQUIRE(view::wildcmp("3*de?33", "3bc12def33"));
        REQUIRE(view::wildcmp("*", "3bc12def33"));
        REQUIRE(view::wildcmp("*", ""));
        REQUIRE(view::wildcmp("3bc12def33", "abc") == false);
    }
}
