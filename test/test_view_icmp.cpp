#include "tester.hpp"

#include "str.hpp"
#include "view.hpp"

TEST_CASE("view::icmp") {
    SECTION("同长字符串对比") {
        REQUIRE(view::icmp("ABCdef", "abcdef") == 0);
        REQUIRE(view::icmp("ABCDEF", "ABCDEF") == 0);
        REQUIRE(view::icmp("abcdef", "ABCDEF") == 0);
        REQUIRE(view::icmp("01234$%#$%#$^", "01234$%#$%#$^") == 0);
        REQUIRE(view::icmp("abcdef", "ABC123") > 0);
    }
    SECTION("长短不一的对比") {
        REQUIRE(view::icmp("abc", "ABCdef") < 0);
        REQUIRE(view::icmp("ABCdef", "abc") > 0);
    }
    SECTION("空串") {
        REQUIRE(view::icmp("", "") == 0);
        REQUIRE(view::icmp("", "abc") < 0);
        REQUIRE(view::icmp("abc", "") > 0);
    }
    SECTION("指定最大对比长度") {
        REQUIRE(view::icmp("", "", 0) == 0);
        REQUIRE(view::icmp("", "abc", 1) < 0);
        REQUIRE(view::icmp("abc", "", 1) > 0);
        REQUIRE(view::icmp("abc", "", 0) == 0);
        REQUIRE(view::icmp("abc", "ABCdef", 3) == 0);
        REQUIRE(view::icmp("ABCdef", "abc", 4) > 0);
        REQUIRE(view::icmp("ABCdef", "abcDEF", 6) == 0);
        REQUIRE(view::icmp("ABCdef", "abcDEF", 7) == 0);
        REQUIRE(view::icmp("ABCdef", "abcDEF", view::npos) == 0);
    }
}
