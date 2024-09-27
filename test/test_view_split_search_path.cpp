#include "catch2/catch_all.hpp"

#include "str.h"
#include "test-compares.h"
#include "view.h"

TEST_CASE("view::split_search_path") {
    SECTION("空串") {
        REQUIRE(view::split_search_path("") == std::vector<std::string_view>{});
    }
    SECTION("空路径") {
        REQUIRE(view::split_search_path(":::") == std::vector<std::string_view>{});
        REQUIRE(view::split_search_path(":::", true) == std::vector<std::string_view>{"", "", ""});

        REQUIRE(view::split_search_path(":  : :") == std::vector<std::string_view>{"  ",  " "});
        REQUIRE(view::split_search_path(":  : :", true) == std::vector<std::string_view>{"", "  ", " ", ""});

        REQUIRE(view::split_search_path(":aaa") == std::vector<std::string_view>{"aaa"});
        REQUIRE(view::split_search_path(":aaa", true) == std::vector<std::string_view>{"", "aaa"});

        REQUIRE(view::split_search_path("abc:def:hij") == std::vector<std::string_view>{"abc", "def", "hij"});
        REQUIRE(view::split_search_path("abc:def:hij", true) == std::vector<std::string_view>{"abc", "def", "hij"});

        REQUIRE(view::split_search_path("abc::hij") == std::vector<std::string_view>{"abc", "hij"});
        REQUIRE(view::split_search_path("abc::hij", true) == std::vector<std::string_view>{"abc", "", "hij"});
    }
}
