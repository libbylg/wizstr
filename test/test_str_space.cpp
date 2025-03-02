#include "tester.hpp"

#include "str.hpp"
#include "view.hpp"

TEST(test_view, space) {
    SECTION("简单字符串重复多次") {
        ASSERT_EQ(view::spaces(5), "     ");
        ASSERT_EQ(view::spaces(0), "");
    }
}

TEST(test_view, skip_space_remain) {
    ASSERT_EQ(view::skip_space_remain("abc"), "abc");
    ASSERT_EQ(view::skip_space_remain("   abc  "), "abc  ");
    ASSERT_EQ(view::skip_space_remain("   abc  ", 1), "abc  ");
    ASSERT_EQ(view::skip_space_remain("   abc  ", 2), "abc  ");
    ASSERT_EQ(view::skip_space_remain("   abc  ", 3), "abc  ");
    ASSERT_EQ(view::skip_space_remain("   abc  ", 10), "");
    ASSERT_EQ(view::skip_space_remain(" \r\t\n ", 0), "");
    ASSERT_EQ(view::skip_space_remain(""), "");
    ASSERT_EQ(view::skip_space_remain("abc  def"), "abc  def");
}

TEST(test_view, skip_space) {
    ASSERT_EQ(view::skip_space("abc", 0), 0);
    ASSERT_EQ(view::skip_space("   abc  ", 0), 3);
    ASSERT_EQ(view::skip_space(" \r\t\n ", 0), 5);
    ASSERT_EQ(view::skip_space("", 0), 0);
    ASSERT_EQ(view::skip_space("abc  def", 7), 7);
    ASSERT_EQ(view::skip_space("abc  def", 8), 8);
    ASSERT_EQ(view::skip_space("abc  def", view::npos), 8);
}
