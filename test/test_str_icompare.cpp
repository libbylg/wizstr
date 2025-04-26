#include "tester.hpp"

#include "str.hpp"


TEST(test_str, icompare) {
    SECTION("同长字符串对比") {
        ASSERT_EQ(str::icompare("ABCdef", "abcdef"), 0);
        ASSERT_EQ(str::icompare("ABCDEF", "ABCDEF"), 0);
        ASSERT_EQ(str::icompare("abcdef", "ABCDEF"), 0);
        ASSERT_EQ(str::icompare("01234$%#$%#$^", "01234$%#$%#$^"), 0);
        ASSERT_TRUE(str::icompare("abcdef", "ABC123") > 0);
    }
    SECTION("长短不一的对比") {
        ASSERT_TRUE(str::icompare("abc", "ABCdef") < 0);
        ASSERT_TRUE(str::icompare("abm", "ABCdef") > 0);
        ASSERT_TRUE(str::icompare("ABCdef", "abc") > 0);
        ASSERT_TRUE(str::icompare("ABadef", "abc") < 0);
    }
    SECTION("空串") {
        ASSERT_EQ(str::icompare("", ""), 0);
        ASSERT_TRUE(str::icompare("", "abc") < 0);
        ASSERT_TRUE(str::icompare("abc", "") > 0);
    }
    SECTION("指定最大对比长度") {
        ASSERT_EQ(str::icompare("", "", 0), 0);
        ASSERT_TRUE(str::icompare("", "abc", 1) < 0);
        ASSERT_TRUE(str::icompare("abc", "", 1) > 0);
        ASSERT_EQ(str::icompare("abc", "", 0), 0);
        ASSERT_EQ(str::icompare("abc", "ABCdef", 3), 0);
        ASSERT_TRUE(str::icompare("ABCdef", "abc", 4) > 0);
        ASSERT_EQ(str::icompare("ABCdef", "abcDEF", 6), 0);
        ASSERT_EQ(str::icompare("ABCdef", "abcDEF", 7), 0);
        ASSERT_EQ(str::icompare("ABCdef", "abcDEF", str::npos), 0);
    }
}
