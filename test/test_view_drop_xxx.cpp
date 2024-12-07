#include "tester.hpp"

#include "str.hpp"
#include "view.hpp"

TEST_CASE("view::drop_left") {
    SECTION("一般场景") {
        REQUIRE(view::drop_left("3bc1233", 0) == "3bc1233");
        REQUIRE(view::drop_left("3bc1233", 3) == "1233");
        REQUIRE(view::drop_left("3bc1233", 7) == "");
        REQUIRE(view::drop_left("3bc1233", 8) == "");
        REQUIRE(view::drop_left("3bc1233", view::npos) == "");
    }
    SECTION("空串") {
        REQUIRE(view::drop_left("", 0) == "");
        REQUIRE(view::drop_left("", 3) == "");
        REQUIRE(view::drop_left("", view::npos) == "");
    }
}

TEST_CASE("view::drop_right") {
    SECTION("一般场景") {
        REQUIRE(view::drop_right("3bc1233", 0) == "3bc1233");
        REQUIRE(view::drop_right("3bc1233", 3) == "3bc1");
        REQUIRE(view::drop_right("3bc1233", 7) == "");
        REQUIRE(view::drop_right("3bc1233", 8) == "");
        REQUIRE(view::drop_right("3bc1233", view::npos) == "");
    }
    SECTION("空串") {
        REQUIRE(view::drop_right("", 0) == "");
        REQUIRE(view::drop_right("", 3) == "");
        REQUIRE(view::drop_right("", view::npos) == "");
    }
}

TEST_CASE("view::drop_mid") {
    SECTION("一般场景") {
        REQUIRE(view::drop_mid("3bc1233", 3, 0) == "3bc1233");
        REQUIRE(view::drop_mid("3bc1233", 2, 3) == "3b33");
        REQUIRE(view::drop_mid("3bc1233", 2, 5) == "3b");
        REQUIRE(view::drop_mid("3bc1233", 2, 6) == "3b");
        REQUIRE(view::drop_mid("3bc1233", 2, view::npos) == "3b");
    }
    SECTION("起始位置") {
        REQUIRE(view::drop_mid("3bc1233", 0, 2) == "c1233");
        REQUIRE(view::drop_mid("3bc1233", 2, 3) == "3b33");
        REQUIRE(view::drop_mid("3bc1234", 6, 1) == "3bc123");
        REQUIRE(view::drop_mid("3bc1234", 6, 2) == "3bc123");
        REQUIRE(view::drop_mid("3bc1234", 7, 1) == "3bc1234");
        REQUIRE(view::drop_mid("3bc1233", view::npos, 6) == "3bc1233");
    }
    SECTION("空串") {
        REQUIRE(view::drop_mid("", 0, 0) == "");
        REQUIRE(view::drop_mid("", 2, 0) == "");
        REQUIRE(view::drop_mid("", 3, view::npos) == "");
    }
}


TEST_CASE("view::drop") {
    SECTION("一般场景") {
        REQUIRE(view::drop("3bc1233", 3, 0) == "3bc1233");
        REQUIRE(view::drop("3bc1233", 2, 3) == "3b33");
        REQUIRE(view::drop("3bc1233", 2, -2) == "31233");
        REQUIRE(view::drop("3bc1233", 2, 5) == "3b");
        REQUIRE(view::drop("3bc1233", 2, -3) == "1233");
        REQUIRE(view::drop("3bc1233", 2, 6) == "3b");
        REQUIRE(view::drop("3bc1233", 2, -4) == "1233");
    }
    SECTION("起始位置") {
        REQUIRE(view::drop("3bc1233", 0, 2) == "c1233");
        REQUIRE(view::drop("3bc1233", 0, -2) == "bc1233");
        REQUIRE(view::drop("3bc1233", 0, 0) == "3bc1233");
        REQUIRE(view::drop("3bc1233", 6, -7) == "");
        REQUIRE(view::drop("3bc1234", 6, 1) == "3bc123");
        REQUIRE(view::drop("3bc1234", 7, -1) == "3bc123");
        REQUIRE(view::drop("3bc1234", 7, -2) == "3bc12");
        REQUIRE(view::drop("3bc1233", view::npos, 6) == "3bc1233");
    }
    SECTION("空串") {
        REQUIRE(view::drop("", 0, 0) == "");
        REQUIRE(view::drop("", 2, -2) == "");
        REQUIRE(view::drop("", 3, view::npos) == "");
    }
}