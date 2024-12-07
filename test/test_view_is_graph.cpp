#include "tester.hpp"

#include "str.hpp"
#include "view.hpp"

TEST(test_view, is_graph) {
    ASSERT_EQ(view::is_graph("0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz"), true);
    ASSERT_EQ(view::is_graph("!@#$%^&*({[<>]})~`_-+="), true);
    ASSERT_EQ(view::is_graph("0123\x7F"), false);
    ASSERT_EQ(view::is_graph("0123 "), false); // 空格比较特殊
    ASSERT_EQ(view::is_graph(""), false);
}
