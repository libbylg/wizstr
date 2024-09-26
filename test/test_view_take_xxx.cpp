#include "catch2/catch_all.hpp"

#include "str.h"
#include "view.h"

TEST_CASE("view::take_left") {
    SECTION("一般场景") {
        REQUIRE(view::take_left("3bc1233", 0) == "");
        REQUIRE(view::take_left("3bc1233", 3) == "3bc");
        REQUIRE(view::take_left("3bc1233", 7) == "3bc1233");
        REQUIRE(view::take_left("3bc1233", 8) == "3bc1233");
        REQUIRE(view::take_left("3bc1233", view::npos) == "3bc1233");
    }
    SECTION("空串") {
        REQUIRE(view::take_left("", 0) == "");
        REQUIRE(view::take_left("", 3) == "");
        REQUIRE(view::take_left("", view::npos) == "");
    }
}

TEST_CASE("view::take_right") {
    SECTION("一般场景") {
        REQUIRE(view::take_right("3bc1233", 0) == "");
        REQUIRE(view::take_right("3bc1233", 3) == "233");
        REQUIRE(view::take_right("3bc1233", 7) == "3bc1233");
        REQUIRE(view::take_right("3bc1233", 8) == "3bc1233");
        REQUIRE(view::take_right("3bc1233", view::npos) == "3bc1233");
    }
    SECTION("空串") {
        REQUIRE(view::take_right("", 0) == "");
        REQUIRE(view::take_right("", 3) == "");
        REQUIRE(view::take_right("", view::npos) == "");
    }
}

TEST_CASE("view::take_mid") {
    SECTION("一般场景") {
        REQUIRE(view::take_mid("3bc1233", 3, 0) == "");
        REQUIRE(view::take_mid("3bc1233", 2, 3) == "c12");
        REQUIRE(view::take_mid("3bc1233", 2, 5) == "c1233");
        REQUIRE(view::take_mid("3bc1233", 2, 6) == "c1233");
        REQUIRE(view::take_mid("3bc1233", 2, view::npos) == "c1233");
    }
    SECTION("起始位置") {
        REQUIRE(view::take_mid("3bc1233", 0, 2) == "3b");
        REQUIRE(view::take_mid("3bc1233", 2, 3) == "c12");
        REQUIRE(view::take_mid("3bc1234", 6, 1) == "4");
        REQUIRE(view::drop_mid("3bc1234", 6, 2) == "4");
        REQUIRE(view::take_mid("3bc1234", 7, 1) == "");
        REQUIRE(view::take_mid("3bc1233", view::npos, 6) == "");
    }
    SECTION("空串") {
        REQUIRE(view::take_mid("", 0, 0) == "");
        REQUIRE(view::take_mid("", 2, 0) == "");
        REQUIRE(view::take_mid("", 3, view::npos) == "");
    }
}


TEST_CASE("view::take") {
    SECTION("一般场景") {
        REQUIRE(view::take("3bc1233", 3, 0) == "");
        REQUIRE(view::take("3bc1233", 2, 3) == "c12");
        REQUIRE(view::take("3bc1233", 2, -2) == "bc");
        REQUIRE(view::take("3bc1233", 2, 5) == "c1233");
        REQUIRE(view::take("3bc1233", 2, -3) == "3bc");
        REQUIRE(view::take("3bc1233", 2, 6) == "c1233");
        REQUIRE(view::take("3bc1233", 2, -4) == "3bc");
    }
    SECTION("起始位置") {
        REQUIRE(view::take("3bc1233", 0, 2) == "3b");
        REQUIRE(view::take("3bc1233", 0, -2) == "3");
        REQUIRE(view::take("3bc1233", 0, 0) == "");
        REQUIRE(view::take("3bc1233", 6, -7) == "3bc1233");
        REQUIRE(view::take("3bc1234", 6, 1) == "4");
        REQUIRE(view::take("3bc1234", 7, -1) == "4");
        REQUIRE(view::take("3bc1234", 7, -2) == "34");
        REQUIRE(view::take("3bc1233", view::npos, 6) == "");
    }
    SECTION("空串") {
        REQUIRE(view::take("", 0, 0) == "");
        REQUIRE(view::take("", 2, -2) == "");
        REQUIRE(view::take("", 3, view::npos) == "");
    }
}