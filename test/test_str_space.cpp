#include "tester.hpp"

#include "str.hpp"

TEST(test_str, space) {
    SECTION("简单字符串重复多次") {
        ASSERT_EQ(str::spaces(5), "     ");
        ASSERT_EQ(str::spaces(0), "");
    }
}

TEST(test_str, skip_space_view) {
    size_t pos{0};
    ASSERT_EQ(str::skip_space_view("abc"), "abc");
    ASSERT_EQ(str::skip_space_view("   abc  "), "abc  ");
    ASSERT_EQ(str::skip_space_view("   abc  ", pos = 1), "abc  ");
    ASSERT_EQ(str::skip_space_view("   abc  ", pos = 2), "abc  ");
    ASSERT_EQ(str::skip_space_view("   abc  ", pos = 3), "abc  ");
    ASSERT_EQ(str::skip_space_view("   abc  ", pos = 10), "");
    ASSERT_EQ(str::skip_space_view(" \r\t\n ", pos = 0), "");
    ASSERT_EQ(str::skip_space_view(""), "");
    ASSERT_EQ(str::skip_space_view("abc  def"), "abc  def");
}

TEST(test_str, skip_space) {
    size_t pos{0};
    ASSERT_EQ(str::skip_space("abc", pos = 0), "abc");
    ASSERT_EQ(pos, 0);
    ASSERT_EQ(str::skip_space("   abc  ", pos = 0), "abc  ");
    ASSERT_EQ(pos, 3);
    ASSERT_EQ(str::skip_space(" \r\t\n ", pos = 0), "");
    ASSERT_EQ(pos, 5);
    ASSERT_EQ(str::skip_space("", pos = 0), "");
    ASSERT_EQ(pos, 0);
    ASSERT_EQ(str::skip_space("abc  def", pos = 7), "");
    ASSERT_EQ(pos, 7);
    ASSERT_EQ(str::skip_space("abc  def", pos = 8), "");
    ASSERT_EQ(pos, 8);
    ASSERT_EQ(str::skip_space("abc  def", pos = str::npos), "");
    ASSERT_EQ(pos, 8);
}
