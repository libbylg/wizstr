#include "catch2/catch_all.hpp"

#include "str.h"
#include "view.h"

//TEST_CASE("view::drop_xxx") {
//    SECTION("一般场景") {
//        REQUIRE(view::drop_left("3bc1233", 0) == "3bc1233");
//        REQUIRE(view::drop_left("3bc1233", 3) == "1233");
//        REQUIRE(view::drop_left("3bc1233", 7) == "");
//        REQUIRE(view::drop_left("3bc1233", 8) == "");
//        REQUIRE(view::drop_left("3bc1233", view::npos) == "");
//    }
//}