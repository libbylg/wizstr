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
    SECTION("指定数量较小") {
        auto result = view::split_map("a:1,b:2,c:3,d:4,e:5", ",", ":", 3);
        auto expect = std::map<std::string, std::string>{{"a", "1"}, {"b", "2"}, {"c", "3"}};
        REQUIRE(result == expect);
    }
    SECTION("指定数量较大1") {
        auto result = view::split_map("a:1,b:2,c:3,d:4,e:5", ",", ":", 5);
        auto expect = std::map<std::string, std::string>{{"a", "1"}, {"b", "2"}, {"c", "3"}, {"d", "4"}, {"e", "5"}};
        REQUIRE(result == expect);
    }
    SECTION("指定数量较大2") {
        auto result = view::split_map("a:1,b:2,c:3,d:4,e:5", ",", ":", 6);
        auto expect = std::map<std::string, std::string>{{"a", "1"}, {"b", "2"}, {"c", "3"}, {"d", "4"}, {"e", "5"}};
        REQUIRE(result == expect);
    }
    SECTION("指定数量较大3") {
        auto result = view::split_map("a:1,b:2,c:3,d:4,e:5", ",", ":", view::npos);
        auto expect = std::map<std::string, std::string>{{"a", "1"}, {"b", "2"}, {"c", "3"}, {"d", "4"}, {"e", "5"}};
        REQUIRE(result == expect);
    }
    SECTION("指定数量为0") {
        auto result = view::split_map("a:1,b:2,c:3,d:4,e:5", ",", ":", 0);
        auto expect = std::map<std::string, std::string>{};
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
    SECTION("分隔空串") {
        auto result = view::split_map(",,,");
        auto expect = std::map<std::string, std::string>{{"", ""}, {"", ""}, {"", ""}};
        REQUIRE(result == expect);
    }
    SECTION("指定分隔为空") {
        auto result = view::split_map("a:1,:2,c:3", "", "");
        auto expect = std::map<std::string, std::string>{{"a", "1"}, {"", "2"}, {"c", "3"}};
        REQUIRE(result == expect);
    }
}