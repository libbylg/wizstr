#include "tester.hpp"

#include "str.hpp"


TEST(test_str, title) {
    ASSERT_EQ(str::title("abc def"), "Abc Def");
    ASSERT_EQ(str::title("   abc def  \r\t\n\v 123"), "   Abc Def  \r\t\n\v 123");
    ASSERT_EQ(str::title("123abc def"), "123Abc Def");
    ASSERT_EQ(str::title(""), "");
    ASSERT_EQ(str::title("z"), "Z");
    ASSERT_EQ(str::title(" q"), " Q");
    ASSERT_EQ(str::title("123456"), "123456");
    ASSERT_EQ(str::title("123Abc Def"), "123Abc Def");
}


TEST(test_str, is_title) {
    ASSERT_EQ(str::is_title("123456"), true);
    ASSERT_EQ(str::is_title(""), true);
    ASSERT_EQ(str::is_title("Abc 123DEF"), true);
    ASSERT_EQ(str::is_title("Abc 123dEF"), false);
    ASSERT_EQ(str::is_title(" abc 123DEF"), false);
    ASSERT_EQ(str::is_title(" Abc Def 123"), true);
    ASSERT_EQ(str::is_title("  A"), true);
    ASSERT_EQ(str::is_title("A"), true);
    ASSERT_EQ(str::is_title("  a"), false);
}
