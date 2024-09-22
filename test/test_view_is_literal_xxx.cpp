#include "catch2/catch_all.hpp"

#include "str.h"
#include "view.h"

TEST_CASE("view::is_literal_bool") {
    SECTION("标准值") {
        REQUIRE(view::is_literal_bool("1"));
        REQUIRE(view::is_literal_bool("0"));
        REQUIRE(view::is_literal_bool("on"));
        REQUIRE(view::is_literal_bool("off"));
        REQUIRE(view::is_literal_bool("ON"));
        REQUIRE(view::is_literal_bool("OFF"));
        REQUIRE(view::is_literal_bool("Yes"));
        REQUIRE(view::is_literal_bool("No"));
        REQUIRE(view::is_literal_bool("yes"));
        REQUIRE(view::is_literal_bool("no"));
        REQUIRE(view::is_literal_bool("YES"));
        REQUIRE(view::is_literal_bool("NO"));
        REQUIRE(view::is_literal_bool("True"));
        REQUIRE(view::is_literal_bool("False"));
        REQUIRE(view::is_literal_bool("true"));
        REQUIRE(view::is_literal_bool("false"));
        REQUIRE(view::is_literal_bool("TRUE"));
        REQUIRE(view::is_literal_bool("FALSE"));
    }
    SECTION("空串") {
        REQUIRE(view::is_literal_bool("") == false);
    }
    SECTION("非标值") {
        REQUIRE(view::is_literal_bool("FalsE") == false);
        REQUIRE(view::is_literal_bool("tRUe") == false);
        REQUIRE(view::is_literal_bool("true ") == false);
    }
    SECTION("超长值") {
        REQUIRE(view::is_literal_bool("hello-workd") == false);
    }
}

TEST_CASE("view::is_literal_true") {
    SECTION("标准值") {
        REQUIRE(view::is_literal_true("1") == true);
        REQUIRE(view::is_literal_true("0") == false);
        REQUIRE(view::is_literal_true("on") == true);
        REQUIRE(view::is_literal_true("off") == false);
        REQUIRE(view::is_literal_true("ON") == true);
        REQUIRE(view::is_literal_true("OFF") == false);
        REQUIRE(view::is_literal_true("Yes") == true);
        REQUIRE(view::is_literal_true("No") == false);
        REQUIRE(view::is_literal_true("yes") == true);
        REQUIRE(view::is_literal_true("no") == false);
        REQUIRE(view::is_literal_true("YES") == true);
        REQUIRE(view::is_literal_true("NO") == false);
        REQUIRE(view::is_literal_true("True") == true);
        REQUIRE(view::is_literal_true("False") == false);
        REQUIRE(view::is_literal_true("true") == true);
        REQUIRE(view::is_literal_true("false") == false);
        REQUIRE(view::is_literal_true("TRUE") == true);
        REQUIRE(view::is_literal_true("FALSE") == false);
    }
    SECTION("空串") {
        REQUIRE(view::is_literal_true("") == false);
    }
    SECTION("非标值") {
        REQUIRE(view::is_literal_true("FalsE") == false);
        REQUIRE(view::is_literal_true("tRUe") == false);
        REQUIRE(view::is_literal_true("true ") == false);
    }
    SECTION("超长值") {
        REQUIRE(view::is_literal_true("hello-workd") == false);
    }
}

TEST_CASE("view::is_literal_false") {
    SECTION("标准值") {
        REQUIRE(view::is_literal_false("1") == false);
        REQUIRE(view::is_literal_false("0") == true);
        REQUIRE(view::is_literal_false("on") == false);
        REQUIRE(view::is_literal_false("off") == true);
        REQUIRE(view::is_literal_false("ON") == false);
        REQUIRE(view::is_literal_false("OFF") == true);
        REQUIRE(view::is_literal_false("Yes") == false);
        REQUIRE(view::is_literal_false("No") == true);
        REQUIRE(view::is_literal_false("yes") == false);
        REQUIRE(view::is_literal_false("no") == true);
        REQUIRE(view::is_literal_false("YES") == false);
        REQUIRE(view::is_literal_false("NO") == true);
        REQUIRE(view::is_literal_false("True") == false);
        REQUIRE(view::is_literal_false("False") == true);
        REQUIRE(view::is_literal_false("true") == false);
        REQUIRE(view::is_literal_false("false") == true);
        REQUIRE(view::is_literal_false("TRUE") == false);
        REQUIRE(view::is_literal_false("FALSE") == true);
    }
    SECTION("空串") {
        REQUIRE(view::is_literal_false("") == false);
    }
    SECTION("非标值") {
        REQUIRE(view::is_literal_false("FalsE") == false);
        REQUIRE(view::is_literal_false("tRUe") == false);
        REQUIRE(view::is_literal_false("true ") == false);
    }
    SECTION("超长值") {
        REQUIRE(view::is_literal_false("hello-workd") == false);
    }
}