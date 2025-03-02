#include "tester.hpp"

#include "str.hpp"
#include "view.hpp"

TEST(test_view, is_alnum) {
    ASSERT_EQ(view::is_alnum("0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz"), true);
    ASSERT_EQ(view::is_alnum("!@#$%^&*({[<>]})~`_-+="), false);
    ASSERT_EQ(view::is_alnum(""), false);
}
