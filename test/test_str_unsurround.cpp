#include "testing.hpp"

#include "str.hpp"

TEST(test_str, unsurround) {
    ASSERT_EQ(str::unsurround("(a+b)", "(", ")"), "a+b");
    ASSERT_EQ(str::unsurround("a+b", "", ""), "a+b");
    ASSERT_EQ(str::unsurround("(a+b)"), "a+b");
    ASSERT_EQ(str::unsurround("()", "(", ")"), "");
    ASSERT_EQ(str::unsurround("(", "(", ""), "");
    ASSERT_EQ(str::unsurround(")", "", ")"), "");
    ASSERT_EQ(str::unsurround("", "", ""), "");
    ASSERT_EQ(str::unsurround("", "{", "}"), "");
    ASSERT_EQ(str::unsurround("((a+b)", "((", "))"), "a+b)");
    ASSERT_EQ(str::unsurround("(a+b))", "((", "))"), "(a+b");
    ASSERT_EQ(str::unsurround("a((+))b", "((", "))"), "a((+))b");
}
