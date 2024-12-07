#include "tester.hpp"

#include "str.hpp"
#include "view.hpp"

TEST(test_view, encode_base16) {
    ASSERT_EQ(view::encode_base16(""), "");
    ASSERT_EQ(view::encode_base16("a"), "61");
    ASSERT_EQ(view::encode_base16("ab"), "6162");
    ASSERT_EQ(view::encode_base16("abc"), "616263");
    ASSERT_EQ(view::encode_base16("abcXYZ", true), "61626358595A");
}


