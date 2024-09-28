#include "catch2/catch_all.hpp"

#include "str.h"
#include "view.h"

#include <list>

TEST_CASE("view::join_search_path") {
    SECTION("一般场景") {
        REQUIRE(view::join_search_path({"A", "B", "C"}) == "A:B:C");
        REQUIRE(view::join_search_path({"A", "B"}) == "A:B");
        REQUIRE(view::join_search_path({"A"}) == "A");
    }
    SECTION("空串") {
        REQUIRE(view::join_search_path({""}) == "");
        REQUIRE(view::join_search_path({"", ""}) == "");
        REQUIRE(view::join_search_path({"", "", "AAA"}) == "AAA");
        REQUIRE(view::join_search_path({"", "A", "B", "", "C", ""}) == "A:B:C");
    }
}

TEST_CASE("view::split_search_path") {
    SECTION("空串") {
        REQUIRE(view::split_search_path("") == std::vector<std::string_view>{});
    }
    SECTION("空路径") {
        REQUIRE(view::split_search_path(":::") == std::vector<std::string_view>{});
        REQUIRE(view::split_search_path(":::", true) == std::vector<std::string_view>{"", "", "", ""});

        REQUIRE(view::split_search_path(":  : :") == std::vector<std::string_view>{"  ", " "});
        REQUIRE(view::split_search_path(":  : :", true) == std::vector<std::string_view>{"", "  ", " ", ""});

        REQUIRE(view::split_search_path(":aaa") == std::vector<std::string_view>{"aaa"});
        REQUIRE(view::split_search_path(":aaa", true) == std::vector<std::string_view>{"", "aaa"});

        REQUIRE(view::split_search_path("abc:def:hij") == std::vector<std::string_view>{"abc", "def", "hij"});
        REQUIRE(view::split_search_path("abc:def:hij", true) == std::vector<std::string_view>{"abc", "def", "hij"});

        REQUIRE(view::split_search_path("abc::hij") == std::vector<std::string_view>{"abc", "hij"});
        REQUIRE(view::split_search_path("abc::hij", true) == std::vector<std::string_view>{"abc", "", "hij"});
    }
}
