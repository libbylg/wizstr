#include "catch2/catch_all.hpp"

#include "str.h"
#include "test-compares.h"
#include "view.h"

#include <map>

TEST_CASE("view::split_map") {
    SECTION("一般情况") {
        auto result = view::split_map("a:1,b:2,c:3");
        auto expect = std::map<std::string, std::string>{{"a", "1"}, {"b", "2"}, {"c", "3"}};
        REQUIRE(result == expect);
    }
    SECTION("空串") {
        auto result = view::split_map("");
        auto expect = std::map<std::string, std::string>{};
        REQUIRE(result == expect);
    }
    SECTION("key为空") {
        auto result = view::split_map("a:1,:2,c:3");
        auto expect = std::map<std::string, std::string>{{"a", "1"}, {"", "2"}, {"c", "3"}};
        REQUIRE(result == expect);
    }
    SECTION("val为空") {
        auto result = view::split_map("a:1,b:2,c:");
        auto expect = std::map<std::string, std::string>{{"a", "1"}, {"b", "2"}, {"c", ""}};
        REQUIRE(result == expect);
    }
    SECTION("key,val均为空") {
        auto result = view::split_map(",,c:");
        auto expect = std::map<std::string, std::string>{{"", ""}, {"", ""}, {"c", ""}};
        REQUIRE(result == expect);
    }
    SECTION("无分隔符") {
        auto result = view::split_map("aa,bb,c:");
        auto expect = std::map<std::string, std::string>{{"aa", ""}, {"bb", ""}, {"c", ""}};
        REQUIRE(result == expect);
    }
}