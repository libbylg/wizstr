#include "testing.hpp"

#include "str.hpp"

TEST(test_str, starts_with_spaces) {
    SECTION("一般情况") {
        ASSERT_FALSE(str::starts_with_spaces("aaa"));
        ASSERT_TRUE(str::starts_with_spaces("\raaa"));
        ASSERT_TRUE(str::starts_with_spaces("     aaa"));
        ASSERT_TRUE(str::starts_with_spaces("\t\raaa"));
        ASSERT_TRUE(str::starts_with_spaces(" "));
    }
    SECTION("空串") {
        ASSERT_FALSE(str::starts_with_spaces(""));
    }
}