#include "tester.hpp"

#include "str.hpp"


TEST(test_str, is_graph) {
    ASSERT_EQ(str::is_graph("0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz"), true);
    ASSERT_EQ(str::is_graph("!@#$%^&*({[<>]})~`_-+="), true);
    ASSERT_EQ(str::is_graph("0123\x7F"), false);
    ASSERT_EQ(str::is_graph("0123 "), false); // 空格比较特殊
    ASSERT_EQ(str::is_graph(""), false);
}
