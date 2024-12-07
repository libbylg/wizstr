#include "tester.hpp"

#include "str.hpp"
#include "view.hpp"

TEST(test_view, is_alpha) {
    ASSERT_EQ(view::is_alpha("ABCDEFGHIUKLMNOPQRSTUVWXYZabcdefghiuklmnopqrstuvwxyz"), true);
    ASSERT_EQ(view::is_alpha("!@#$%^&*({[<>]})~`_-+="), false);
    ASSERT_EQ(view::is_alpha(""), false);
}
