#include "tester.hpp"

#include "str.hpp"


TEST(test_str, icmp) {
    SECTION("同长字符串对比") {
        ASSERT_EQ(str::icmp("ABCdef", "abcdef"), 0);
        ASSERT_EQ(str::icmp("ABCDEF", "ABCDEF"), 0);
        ASSERT_EQ(str::icmp("abcdef", "ABCDEF"), 0);
        ASSERT_EQ(str::icmp("01234$%#$%#$^", "01234$%#$%#$^"), 0);
        ASSERT_TRUE(str::icmp("abcdef", "ABC123") > 0);
    }
    SECTION("长短不一的对比") {
        ASSERT_TRUE(str::icmp("abc", "ABCdef") < 0);
        ASSERT_TRUE(str::icmp("ABCdef", "abc") > 0);
    }
    SECTION("空串") {
        ASSERT_EQ(str::icmp("", ""), 0);
        ASSERT_TRUE(str::icmp("", "abc") < 0);
        ASSERT_TRUE(str::icmp("abc", "") > 0);
    }
    SECTION("指定最大对比长度") {
        ASSERT_EQ(str::icmp("", "", 0), 0);
        ASSERT_TRUE(str::icmp("", "abc", 1) < 0);
        ASSERT_TRUE(str::icmp("abc", "", 1) > 0);
        ASSERT_EQ(str::icmp("abc", "", 0), 0);
        ASSERT_EQ(str::icmp("abc", "ABCdef", 3), 0);
        ASSERT_TRUE(str::icmp("ABCdef", "abc", 4) > 0);
        ASSERT_EQ(str::icmp("ABCdef", "abcDEF", 6), 0);
        ASSERT_EQ(str::icmp("ABCdef", "abcDEF", 7), 0);
        ASSERT_EQ(str::icmp("ABCdef", "abcDEF", str::npos), 0);
    }
}
