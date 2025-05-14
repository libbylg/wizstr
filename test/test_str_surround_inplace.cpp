#include "testing.hpp"

#include "str.hpp"

TEST(test_str, surround_inplace) {
    std::string s;
    ASSERT_EQ(str::surround_inplace(s = "a+b", "(", ")"), "(a+b)");
    ASSERT_EQ(str::surround_inplace(s = "a+b", "", ""), "a+b");
    ASSERT_EQ(str::surround_inplace(s = "a+b"), "(a+b)");
    ASSERT_EQ(str::surround_inplace(s = "", "(", ")"), "()");
    ASSERT_EQ(str::surround_inplace(s = "", "(", ""), "(");
    ASSERT_EQ(str::surround_inplace(s = "", "", ")"), ")");
    ASSERT_EQ(str::surround_inplace(s = "", "", ""), "");
}
