#include "tester.hpp"

#include "str.hpp"
#include "view.hpp"

TEST(test_view, is_lower) {
    SECTION("全小写") {
        ASSERT_EQ(view::is_lower("abcdef"), true);
    }
    SECTION("只有一个大写") {
        ASSERT_EQ(view::is_lower("abcdeF"), false);
    }
    SECTION("无字母") {
        ASSERT_EQ(view::is_lower("092320914#$@#$@35"), false);
    }
    SECTION("混合") {
        ASSERT_EQ(view::is_lower("abcDEF0923"), false);
    }
    SECTION("空串") {
        ASSERT_EQ(view::is_lower(""), false);
    }
}

TEST(test_view, is_upper) {
    SECTION("全大写") {
        ASSERT_EQ(view::is_upper("ABCDEF"), true);
    }
    SECTION("只有一个小写") {
        ASSERT_EQ(view::is_upper("ABCDEf"), false);
    }
    SECTION("无字母") {
        ASSERT_EQ(view::is_upper("092320914#$@#$@35"), false);
    }
    SECTION("混合") {
        ASSERT_EQ(view::is_upper("abcDEF0923"), false);
    }
    SECTION("空串") {
        ASSERT_EQ(view::is_upper(""), false);
    }
}