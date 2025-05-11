#include "testing.hpp"

#include "str.hpp"

TEST(test_str, swap_case) {
    SECTION("一般情况") {
        ASSERT_EQ(str::swap_case("  AabbCC中华人民共和国"), "  aABBcc中华人民共和国");
        ASSERT_EQ(str::swap_case("中华人民共和国"), "中华人民共和国");
        ASSERT_EQ(str::swap_case("0123456789*&^%#$@"), "0123456789*&^%#$@");
    }
    SECTION("空串") {
        ASSERT_EQ(str::swap_case(""), "");
    }
    SECTION("全部是大写或者小写") {
        ASSERT_EQ(str::swap_case("ABCDEFGHIJK"), "abcdefghijk");
        ASSERT_EQ(str::swap_case("abcdefghijk"), "ABCDEFGHIJK");
    }
}
