#include "testing.hpp"

#include "str.hpp"


TEST(test_str, is_lower) {
    SECTION("全小写") {
        ASSERT_EQ(str::is_lower("abcdef"), true);
    }
    SECTION("只有一个大写") {
        ASSERT_EQ(str::is_lower("abcdeF"), false);
    }
    SECTION("无字母") {
        ASSERT_EQ(str::is_lower("092320914#$@#$@35"), false);
    }
    SECTION("混合") {
        ASSERT_EQ(str::is_lower("abcDEF0923"), false);
    }
    SECTION("空串") {
        ASSERT_EQ(str::is_lower(""), false);
    }
}

TEST(test_str, is_upper) {
    SECTION("全大写") {
        ASSERT_EQ(str::is_upper("ABCDEF"), true);
    }
    SECTION("只有一个小写") {
        ASSERT_EQ(str::is_upper("ABCDEf"), false);
    }
    SECTION("无字母") {
        ASSERT_EQ(str::is_upper("092320914#$@#$@35"), false);
    }
    SECTION("混合") {
        ASSERT_EQ(str::is_upper("abcDEF0923"), false);
    }
    SECTION("空串") {
        ASSERT_EQ(str::is_upper(""), false);
    }
}