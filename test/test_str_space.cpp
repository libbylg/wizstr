#include "testing.hpp"

#include "str.hpp"

TEST(test_str, make_spaces) {
    SECTION("简单字符串重复多次") {
        ASSERT_EQ(str::make_spaces(5), "     ");
        ASSERT_EQ(str::make_spaces(0), "");
    }
}

TEST(test_str, skip_space_view) {
    size_t pos{0};
    ASSERT_EQ(str::after_skip_spaces_view("abc"), "abc");
    ASSERT_EQ(str::after_skip_spaces_view("   abc  "), "abc  ");
    ASSERT_EQ(str::after_skip_spaces_view("   abc  ", pos = 1), "abc  ");
    ASSERT_EQ(str::after_skip_spaces_view("   abc  ", pos = 2), "abc  ");
    ASSERT_EQ(str::after_skip_spaces_view("   abc  ", pos = 3), "abc  ");
    ASSERT_EQ(str::after_skip_spaces_view("   abc  ", pos = 10), "");
    ASSERT_EQ(str::after_skip_spaces_view(" \r\t\n ", pos = 0), "");
    ASSERT_EQ(str::after_skip_spaces_view(""), "");
    ASSERT_EQ(str::after_skip_spaces_view("abc  def"), "abc  def");
}

TEST(test_str, skip_space) {
    size_t pos{0};
    ASSERT_EQ(str::after_skip_spaces("abc", pos = 0), "abc");
    //ASSERT_EQ(pos, 0);
    ASSERT_EQ(str::after_skip_spaces("   abc  ", pos = 0), "abc  ");
    //ASSERT_EQ(pos, 3);
    ASSERT_EQ(str::after_skip_spaces(" \r\t\n ", pos = 0), "");
    //ASSERT_EQ(pos, 5);
    ASSERT_EQ(str::after_skip_spaces("", pos = 0), "");
    //ASSERT_EQ(pos, 0);
    ASSERT_EQ(str::after_skip_spaces("abc  def", pos = 7), "f");
    //ASSERT_EQ(pos, 7);
    ASSERT_EQ(str::after_skip_spaces("abc  def", pos = 8), "");
    //ASSERT_EQ(pos, 8);
    ASSERT_EQ(str::after_skip_spaces("abc  def", pos = str::npos), "");
    //ASSERT_EQ(pos, 8);
}
