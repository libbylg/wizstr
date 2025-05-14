#include "testing.hpp"

#include "str.hpp"

TEST(test_str, surround) {
    ASSERT_EQ(str::surround("a+b", "(", ")"), "(a+b)");
    ASSERT_EQ(str::surround("a+b", "", ""), "a+b");
    ASSERT_EQ(str::surround("a+b"), "(a+b)");
    ASSERT_EQ(str::surround("", "(", ")"), "()");
    ASSERT_EQ(str::surround("", "(", ""), "(");
    ASSERT_EQ(str::surround("", "", ")"), ")");
    ASSERT_EQ(str::surround("", "", ""), "");
}
