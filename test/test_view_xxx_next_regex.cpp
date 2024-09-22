#include "catch2/catch_all.hpp"

#include "str.h"
#include "view.h"

TEST_CASE("view::find_next_regex") {
    SECTION("不同起始位置") {
        REQUIRE(view::find_next_regex("abc1234def", "[0-9]+") == std::string("1234"));
    }
}