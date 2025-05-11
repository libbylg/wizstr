#include "testing.hpp"

#include "str.hpp"

TEST(test_str, to_lower) {
    SECTION("一般情况") {
        ASSERT_EQ(str::to_lower("  AabbCC中华人民共和国"), "  aabbcc中华人民共和国");
        ASSERT_EQ(str::to_lower("中华人民共和国"), "中华人民共和国");
        ASSERT_EQ(str::to_lower("0123456789*&^%#$@"), "0123456789*&^%#$@");
    }
    SECTION("空串") {
        ASSERT_EQ(str::to_lower(""), "");
    }
    SECTION("全部是大写或者小写") {
        ASSERT_EQ(str::to_lower("ABCDEFGHIJK"), "abcdefghijk");
        ASSERT_EQ(str::to_lower("abcdefghijk"), "abcdefghijk");
    }
}