#include "testing.hpp"

#include "str.hpp"

TEST(test_str, unsurround_inplace) {
    std::string s;
    ASSERT_EQ(str::unsurround_inplace(s = "(a+b)", "(", ")"), "a+b");
    ASSERT_EQ(str::unsurround_inplace(s = "a+b", "", ""), "a+b");
    ASSERT_EQ(str::unsurround_inplace(s = "(a+b)"), "a+b");
    ASSERT_EQ(str::unsurround_inplace(s = "()", "(", ")"), "");
    ASSERT_EQ(str::unsurround_inplace(s = "(", "(", ""), "");
    ASSERT_EQ(str::unsurround_inplace(s = ")", "", ")"), "");
    ASSERT_EQ(str::unsurround_inplace(s = "", "", ""), "");
    ASSERT_EQ(str::unsurround_inplace(s = "", "{", "}"), "");
    ASSERT_EQ(str::unsurround_inplace(s = "((a+b)", "((", "))"), "a+b)");
    ASSERT_EQ(str::unsurround_inplace(s = "(a+b))", "((", "))"), "(a+b");
    ASSERT_EQ(str::unsurround_inplace(s = "a((+))b", "((", "))"), "a((+))b");
}
