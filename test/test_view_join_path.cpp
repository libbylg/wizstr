#include "catch2/catch_all.hpp"

#include "str.h"
#include "view.h"

#include <list>

TEST_CASE("view::join_path") {
    SECTION("一般场景") {
        REQUIRE(view::join_path(std::vector{"A", "B", "C"}) == "A/B/C");
    }
    SECTION("空") {
        REQUIRE(view::join_path(std::vector<std::string_view>{}) == "");
        REQUIRE(view::join_path(std::vector<std::string_view>{""}) == "");
        REQUIRE(view::join_path(std::vector<std::string_view>{"", "", "abc"}) == "abc");
        REQUIRE(view::join_path(std::vector<std::string_view>{"", "", "abc", ""}) == "abc");
        REQUIRE(view::join_path(std::vector<std::string_view>{"abc", "", "", "def"}) == "abc/def");
    }
    SECTION("绝对路径") {
        REQUIRE(view::join_path(std::list<std::string_view>{"/"}) == "/");
        REQUIRE(view::join_path(std::list<std::string_view>{"/", ""}) == "/");
        REQUIRE(view::join_path(std::list<std::string_view>{"aaa", "/bbb", "ccc"}) == "/bbb/ccc");
        REQUIRE(view::join_path(std::list<std::string_view>{"/", "abc", "/def", "ghi"}) == "/def/ghi");
    }
    SECTION("initialize_list") {
        REQUIRE(view::join_path({"aaa", "bbb", "ccc"}) == "aaa/bbb/ccc");
    }
}

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