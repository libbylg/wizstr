#include "tester.hpp"

#include "str.hpp"
#include "view.hpp"

TEST(test_view, is_print) {
    ASSERT_EQ(view::is_print("0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz"), true);
    ASSERT_EQ(view::is_print("!@#$%^&*({[<>]})~`_-+= "), true);
    ASSERT_EQ(view::is_print(""), false);
    ASSERT_EQ(view::is_print("abc123 \1\2\3\4"), false);
}
