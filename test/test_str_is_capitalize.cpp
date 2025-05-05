#include "testing.hpp"

#include "str.hpp"


TEST(test_str, is_capitalize) {
    SECTION("一般情况") {
        ASSERT_EQ(str::is_capitalize("A"), true);
        ASSERT_EQ(str::is_capitalize("This Is Hello World"), true);
        ASSERT_EQ(str::is_capitalize("this is hello world"), false);
        ASSERT_EQ(str::is_capitalize("THIS IS HELLO world"), true);
        ASSERT_EQ(str::is_capitalize("123 456 789"), false);
        ASSERT_EQ(str::is_capitalize("123A 456A 789A"), false);
        ASSERT_EQ(str::is_capitalize("0"), false);
        ASSERT_EQ(str::is_capitalize(""), false);
    }
    SECTION("空格") {
        ASSERT_EQ(str::is_capitalize(" \t\r\n\vA \t\r\n\v"), false);
        ASSERT_EQ(str::is_capitalize(" \t\r\n\v"), false);
    }
}
