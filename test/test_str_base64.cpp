#include "tester.hpp"

#include "str.hpp"
#include "view.hpp"

TEST(test_view, encode_base64) {
    ASSERT_EQ(view::encode_base64(""), "");
    ASSERT_EQ(view::encode_base64("a"), "YQ==");
    ASSERT_EQ(view::encode_base64("ab"), "YWI=");
    ASSERT_EQ(view::encode_base64("abc"), "YWJj");
    ASSERT_EQ(view::encode_base64("abcdef"), "YWJjZGVm");
}


