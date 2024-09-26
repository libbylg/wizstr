#include "catch2/catch_all.hpp"

#include "str.h"
#include "view.h"

TEST_CASE("view::expand_user") {
    SECTION("简单场景") {
        REQUIRE(view::expand_user("~") == std::string{getenv("HOME")});
        REQUIRE(view::expand_user("~/") == std::string{getenv("HOME")} + "/");
        REQUIRE(view::expand_user("~/abc") == std::string{getenv("HOME")} + "/abc");
    }
    SECTION("空串") {
        REQUIRE(view::expand_user("") == "");
    }
    SECTION("起始点不匹配") {
        REQUIRE(view::expand_user("~ ") == "~ ");
        REQUIRE(view::expand_user(" ~/abc") == " ~/abc");
    }
}
