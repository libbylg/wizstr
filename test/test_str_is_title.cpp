#include "testing.hpp"

#include "str.hpp"

TEST(test_str, is_title) {
    SECTION("一般情况") {
        ASSERT_EQ(str::is_title("A"), true);
        ASSERT_EQ(str::is_title("This Is Hello World"), true);
        ASSERT_EQ(str::is_title("this is hello world"), false);
        ASSERT_EQ(str::is_title("THIS IS HELLO world"), false);
        ASSERT_EQ(str::is_title("123 456 789"), false);
        ASSERT_EQ(str::is_title("123A 456A 789A"), true);
        ASSERT_EQ(str::is_title("0"), false);

        ASSERT_EQ(str::is_title(""), false);
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
    SECTION("空格") {
        ASSERT_EQ(str::is_title(" \t\r\n\vA \t\r\n\v"), true);
        ASSERT_EQ(str::is_title(" \t\r\n\v"), false);
    }
}

