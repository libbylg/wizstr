#include "testing.hpp"

#include "str.hpp"


TEST(test_str, is_alpha) {
    ASSERT_EQ(str::is_alpha("ABCDEFGHIUKLMNOPQRSTUVWXYZabcdefghiuklmnopqrstuvwxyz"), true);
    ASSERT_EQ(str::is_alpha("!@#$%^&*({[<>]})~`_-+="), false);
    ASSERT_EQ(str::is_alpha(""), false);
}
