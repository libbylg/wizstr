#include "tester.hpp"

#include "str.hpp"
#include "view.hpp"

TEST(test_view, title) {
    ASSERT_EQ(view::title("abc def"), "Abc Def");
    ASSERT_EQ(view::title("   abc def  \r\t\n\v 123"), "   Abc Def  \r\t\n\v 123");
    ASSERT_EQ(view::title("123abc def"), "123Abc Def");
    ASSERT_EQ(view::title(""), "");
    ASSERT_EQ(view::title("z"), "Z");
    ASSERT_EQ(view::title(" q"), " Q");
    ASSERT_EQ(view::title("123456"), "123456");
    ASSERT_EQ(view::title("123Abc Def"), "123Abc Def");
}


TEST(test_view, is_title) {
    ASSERT_EQ(view::is_title("123456"), true);
    ASSERT_EQ(view::is_title(""), true);
    ASSERT_EQ(view::is_title("Abc 123DEF"), true);
    ASSERT_EQ(view::is_title("Abc 123dEF"), false);
    ASSERT_EQ(view::is_title(" abc 123DEF"), false);
    ASSERT_EQ(view::is_title(" Abc Def 123"), true);
    ASSERT_EQ(view::is_title("  A"), true);
    ASSERT_EQ(view::is_title("A"), true);
    ASSERT_EQ(view::is_title("  a"), false);
}
