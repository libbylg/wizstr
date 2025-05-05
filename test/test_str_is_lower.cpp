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
