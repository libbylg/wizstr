#include "tester.hpp"

#include "str.hpp"
#include "view.hpp"

TEST(test_view, repeat) {
    SECTION("简单字符串重复多次") {
        ASSERT_EQ(view::repeat("0123456789", 5), "01234567890123456789012345678901234567890123456789");
        ASSERT_EQ(view::repeat(' ', 5), "     ");
    }
    SECTION("对空字符串重复：仍然得到的是空字符串") {
        ASSERT_EQ(view::repeat("", 5), "");
        ASSERT_EQ(view::repeat("", 4096), "");
    }
    SECTION("对任意字符串重复0次：仍然是空字符串") {
        ASSERT_EQ(view::repeat("", 0), "");
        ASSERT_EQ(view::repeat("abc", 0), "");
        ASSERT_EQ(view::repeat('A', 0), "");
    }
}


