#include "catch2/catch_all.hpp"

#include "str.h"
#include "view.h"

TEST_CASE("view::iter_next_string") {
    SECTION("一般场景") {
        std::string_view s{"AAabcAAdefAAAA"};
        size_t pos = 0;

        REQUIRE(view::iter_next_string(s, pos, "AA") == 0);
        REQUIRE(pos == 2);

        REQUIRE(view::iter_next_string(s, pos, "AA") == 5);
        REQUIRE(pos == 7);

        REQUIRE(view::iter_next_string(s, pos, "AA") == 10);
        REQUIRE(pos == 12);

        REQUIRE(view::iter_next_string(s, pos, "AA") == 12);
        REQUIRE(pos == 14);

        REQUIRE(view::iter_next_string(s, pos, "AA") == view::npos);
        REQUIRE(pos == 14);

        REQUIRE(view::iter_next_string(s, pos, "AA") == view::npos);
        REQUIRE(pos == 14);
    }
    SECTION("pos超过范围:1") {
        std::string_view s{"AAabcAAdefAAAA"};
        size_t pos = 14;

        REQUIRE(view::iter_next_string(s, pos, "AA") == view::npos);
        REQUIRE(pos == 14);
    }
    SECTION("pos超过范围:npos") {
        std::string_view s{"AAabcAAdefAAAA"};
        size_t pos = view::npos;

        REQUIRE(view::iter_next_string(s, pos, "AA") == view::npos);
        REQUIRE(pos == 14);
    }
    SECTION("空串") {
        std::string_view s{""};
        size_t pos = 14;

        REQUIRE(view::iter_next_string(s, pos, "AA") == view::npos);
        REQUIRE(pos == 0);
    }
    SECTION("空串") {
        std::string_view s{"AACDEF"};
        size_t pos = 0;

        REQUIRE(view::iter_next_string(s, pos, "AA") == 0);
        REQUIRE(pos == 2);

        REQUIRE(view::iter_next_string(s, pos, "AA") == view::npos);
        REQUIRE(pos == 6);
    }
}