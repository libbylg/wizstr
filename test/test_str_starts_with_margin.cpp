#include "testing.hpp"

#include "str.hpp"

TEST(test_str, starts_with_margin) {
    SECTION("一般情况") {
        ASSERT_TRUE(str::starts_with_margin("|aaa", '|'));
        ASSERT_TRUE(str::starts_with_margin("\r|aaa", '|'));
        ASSERT_TRUE(str::starts_with_margin("     |aaa", '|'));
        ASSERT_TRUE(str::starts_with_margin("\t\r|aaa", '|'));
        ASSERT_TRUE(str::starts_with_margin(" |", '|'));
        ASSERT_FALSE(str::starts_with_margin(" ", '*'));
        ASSERT_FALSE(str::starts_with_margin(" ", ' '));
    }
    SECTION("空串") {
        ASSERT_FALSE(str::starts_with_margin("", '|'));
    }
}