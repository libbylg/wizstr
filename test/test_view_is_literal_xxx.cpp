#include "tester.hpp"

#include "str.hpp"
#include "view.hpp"

TEST(test_view, is_literal_bool) {
    SECTION("标准值") {
        ASSERT_TRUE(view::is_literal_bool("1"));
        ASSERT_TRUE(view::is_literal_bool("0"));
        ASSERT_TRUE(view::is_literal_bool("on"));
        ASSERT_TRUE(view::is_literal_bool("off"));
        ASSERT_TRUE(view::is_literal_bool("ON"));
        ASSERT_TRUE(view::is_literal_bool("OFF"));
        ASSERT_TRUE(view::is_literal_bool("On"));
        ASSERT_TRUE(view::is_literal_bool("Off"));
        ASSERT_TRUE(view::is_literal_bool("Yes"));
        ASSERT_TRUE(view::is_literal_bool("No"));
        ASSERT_TRUE(view::is_literal_bool("yes"));
        ASSERT_TRUE(view::is_literal_bool("no"));
        ASSERT_TRUE(view::is_literal_bool("YES"));
        ASSERT_TRUE(view::is_literal_bool("NO"));
        ASSERT_TRUE(view::is_literal_bool("True"));
        ASSERT_TRUE(view::is_literal_bool("False"));
        ASSERT_TRUE(view::is_literal_bool("true"));
        ASSERT_TRUE(view::is_literal_bool("false"));
        ASSERT_TRUE(view::is_literal_bool("TRUE"));
        ASSERT_TRUE(view::is_literal_bool("FALSE"));
    }
    SECTION("空串") {
        ASSERT_EQ(view::is_literal_bool(""), false);
    }
    SECTION("非标值") {
        ASSERT_EQ(view::is_literal_bool("FalsE"), false);
        ASSERT_EQ(view::is_literal_bool("tRUe"), false);
        ASSERT_EQ(view::is_literal_bool("true "), false);
    }
    SECTION("超长值") {
        ASSERT_EQ(view::is_literal_bool("hello-workd"), false);
    }
}

TEST(test_view, is_literal_true) {
    SECTION("标准值") {
        ASSERT_EQ(view::is_literal_true("1"), true);
        ASSERT_EQ(view::is_literal_true("0"), false);
        ASSERT_EQ(view::is_literal_true("on"), true);
        ASSERT_EQ(view::is_literal_true("off"), false);
        ASSERT_EQ(view::is_literal_true("ON"), true);
        ASSERT_EQ(view::is_literal_true("OFF"), false);
        ASSERT_EQ(view::is_literal_true("On"), true);
        ASSERT_EQ(view::is_literal_true("Off"), false);
        ASSERT_EQ(view::is_literal_true("Yes"), true);
        ASSERT_EQ(view::is_literal_true("No"), false);
        ASSERT_EQ(view::is_literal_true("yes"), true);
        ASSERT_EQ(view::is_literal_true("no"), false);
        ASSERT_EQ(view::is_literal_true("YES"), true);
        ASSERT_EQ(view::is_literal_true("NO"), false);
        ASSERT_EQ(view::is_literal_true("True"), true);
        ASSERT_EQ(view::is_literal_true("False"), false);
        ASSERT_EQ(view::is_literal_true("true"), true);
        ASSERT_EQ(view::is_literal_true("false"), false);
        ASSERT_EQ(view::is_literal_true("TRUE"), true);
        ASSERT_EQ(view::is_literal_true("FALSE"), false);
    }
    SECTION("空串") {
        ASSERT_EQ(view::is_literal_true(""), false);
    }
    SECTION("非标值") {
        ASSERT_EQ(view::is_literal_true("FalsE"), false);
        ASSERT_EQ(view::is_literal_true("tRUe"), false);
        ASSERT_EQ(view::is_literal_true("true "), false);
    }
    SECTION("超长值") {
        ASSERT_EQ(view::is_literal_true("hello-workd"), false);
    }
}

TEST(test_view, is_literal_false) {
    SECTION("标准值") {
        ASSERT_EQ(view::is_literal_false("1"), false);
        ASSERT_EQ(view::is_literal_false("0"), true);
        ASSERT_EQ(view::is_literal_false("on"), false);
        ASSERT_EQ(view::is_literal_false("off"), true);
        ASSERT_EQ(view::is_literal_false("ON"), false);
        ASSERT_EQ(view::is_literal_false("OFF"), true);
        ASSERT_EQ(view::is_literal_false("On"), false);
        ASSERT_EQ(view::is_literal_false("Off"), true);
        ASSERT_EQ(view::is_literal_false("Yes"), false);
        ASSERT_EQ(view::is_literal_false("No"), true);
        ASSERT_EQ(view::is_literal_false("yes"), false);
        ASSERT_EQ(view::is_literal_false("no"), true);
        ASSERT_EQ(view::is_literal_false("YES"), false);
        ASSERT_EQ(view::is_literal_false("NO"), true);
        ASSERT_EQ(view::is_literal_false("True"), false);
        ASSERT_EQ(view::is_literal_false("False"), true);
        ASSERT_EQ(view::is_literal_false("true"), false);
        ASSERT_EQ(view::is_literal_false("false"), true);
        ASSERT_EQ(view::is_literal_false("TRUE"), false);
        ASSERT_EQ(view::is_literal_false("FALSE"), true);
    }
    SECTION("空串") {
        ASSERT_EQ(view::is_literal_false(""), false);
    }
    SECTION("非标值") {
        ASSERT_EQ(view::is_literal_false("FalsE"), false);
        ASSERT_EQ(view::is_literal_false("tRUe"), false);
        ASSERT_EQ(view::is_literal_false("true "), false);
    }
    SECTION("超长值") {
        ASSERT_EQ(view::is_literal_false("hello-workd"), false);
    }
}