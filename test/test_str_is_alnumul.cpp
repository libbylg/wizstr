#include "testing.hpp"

#include "str.hpp"


TEST(test_str, is_alnumul) {
    ASSERT_EQ(str::is_alnumul("_0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz"), true);
    ASSERT_EQ(str::is_alnumul("!@#$%^&*({[<>]})~`-+="), false);
    ASSERT_EQ(str::is_alnumul(""), false);
}
