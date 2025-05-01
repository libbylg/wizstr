#include "testing.hpp"

#include "str.hpp"


TEST(test_str, encode_base64) {
    ASSERT_EQ(str::encode_base64(""), "");
    ASSERT_EQ(str::encode_base64("a"), "YQ==");
    ASSERT_EQ(str::encode_base64("ab"), "YWI=");
    ASSERT_EQ(str::encode_base64("abc"), "YWJj");
    ASSERT_EQ(str::encode_base64("abcdef"), "YWJjZGVm");
}


