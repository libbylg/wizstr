#include "testing.hpp"

#include "str.hpp"

TEST(test_str, to_upper) {
    SECTION("一般情况") {
        ASSERT_EQ(str::to_upper("  AabbCC中华人民共和国"), "  AABBCC中华人民共和国");
        ASSERT_EQ(str::to_upper("中华人民共和国"), "中华人民共和国");
        ASSERT_EQ(str::to_upper("0123456789*&^%#$@"), "0123456789*&^%#$@");
    }
    SECTION("空串") {
        ASSERT_EQ(str::to_upper(""), "");
    }
    SECTION("全部是大写或者小写") {
        ASSERT_EQ(str::to_upper("ABCDEFGHIJK"), "ABCDEFGHIJK");
        ASSERT_EQ(str::to_upper("abcdefghijk"), "ABCDEFGHIJK");
    }
}
