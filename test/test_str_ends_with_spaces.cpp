#include "testing.hpp"

#include "str.hpp"

TEST(test_str, ends_with_spaces) {
    SECTION("一般情况") {
        ASSERT_FALSE(str::ends_with_spaces("aaa"));
        ASSERT_TRUE(str::ends_with_spaces("aaa\r"));
        ASSERT_TRUE(str::ends_with_spaces("aaa     "));
        ASSERT_TRUE(str::ends_with_spaces("aaa\t\r"));
        ASSERT_TRUE(str::ends_with_spaces(" "));
    }
    SECTION("空串") {
        ASSERT_FALSE(str::ends_with_spaces(""));
    }
}