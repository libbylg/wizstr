#include "tester.hpp"

#include "str.hpp"


TEST(test_str, is_print) {
    ASSERT_EQ(str::is_print("0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz"), true);
    ASSERT_EQ(str::is_print("!@#$%^&*({[<>]})~`_-+= "), true);
    ASSERT_EQ(str::is_print(""), false);
    ASSERT_EQ(str::is_print("abc123 \1\2\3\4"), false);
}
