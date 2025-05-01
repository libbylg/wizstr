#include "testing.hpp"

#include "str.hpp"


TEST(test_str, is_alnum) {
    ASSERT_EQ(str::is_alnum("0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz"), true);
    ASSERT_EQ(str::is_alnum("!@#$%^&*({[<>]})~`_-+="), false);
    ASSERT_EQ(str::is_alnum(""), false);
}
