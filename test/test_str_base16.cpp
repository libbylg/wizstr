#include "tester.hpp"

#include "str.hpp"


TEST(test_str, encode_base16) {
    ASSERT_EQ(str::encode_base16(""), "");
    ASSERT_EQ(str::encode_base16("a"), "61");
    ASSERT_EQ(str::encode_base16("ab"), "6162");
    ASSERT_EQ(str::encode_base16("abc"), "616263");
    ASSERT_EQ(str::encode_base16("abcXYZ", true), "61626358595A");
}


