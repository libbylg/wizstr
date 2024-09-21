#include "catch2/catch_all.hpp"

#include "str.h"
#include "view.h"

#include <array>
#include <list>
#include <vector>

TEST_CASE("view::join_list") {
    SECTION("一般场景") {
        REQUIRE(view::join_list(std::vector{"A", "B", "C"}) == "A,B,C");
        REQUIRE(view::join_list(";", std::vector{"A", "B"}) == "A;B");
        REQUIRE(view::join_list("#", std::vector{"A"}) == "A");
        REQUIRE(view::join_list("|", std::vector{"A", "B", "C"}) == "A|B|C");
        REQUIRE(view::join_list("", std::vector{"A", "B", "C"}) == "ABC");
        REQUIRE(view::join_list("|", std::vector<view::const_pointer>{}) == "");
        REQUIRE(view::join_list("|", std::vector{"A", "", "C"}) == "A||C");
        REQUIRE(view::join_list("/", std::list{"A", "B", "C"}) == "A/B/C");
        REQUIRE(view::join_list("*", std::array{"A", "B", "C"}) == "A*B*C");
        REQUIRE(view::join_list("*", std::array{"", "", ""}) == "**");
    }
    SECTION("proc使用供给数据") {
        std::array items{"A", "B", "C"};
        size_t i = 0;
        REQUIRE(view::join_list([&i, &items]() -> std::optional<std::string_view> {
            if (i >= items.size()) {
                return std::nullopt;
            }

            return items[i++];
        }) == "A,B,C");
    }
}
