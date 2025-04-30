#include "tester.hpp"

#include "str.hpp"

TEST(test_str, icontains) {
    SECTION("简单场景") {
        ASSERT_EQ(str::icontains("3bc1233", "3bc"), true);
        ASSERT_EQ(str::icontains("3bc1233", "12"), true);
        ASSERT_EQ(str::icontains("bc1233", "33"), true);
        ASSERT_EQ(str::icontains("bc1233", "31"), false);
        ASSERT_EQ(str::icontains("bc1233", "bc1233"), true);
    }
    SECTION("空串") {
        ASSERT_EQ(str::icontains("", ""), true);
        ASSERT_EQ(str::icontains("abc", ""), true);
        ASSERT_EQ(str::icontains("", "abc"), false);
    }
    SECTION("针对字符") {
        ASSERT_EQ(str::icontains(std::string_view{"abc\0def", 7}, '\0'), true); // 指定长度时，\0 被包含在内
        ASSERT_EQ(str::icontains("abc", '\0'), false);                          // "abc" 转换为 std::string_view 时，并不会包含 \0 字符
        ASSERT_EQ(str::icontains("abc", 'a'), true);
        ASSERT_EQ(str::icontains("abc", 'b'), true);
        ASSERT_EQ(str::icontains("abc", 'c'), true);
    }
    SECTION("大小写") {
        ASSERT_EQ(str::icontains("ABC", 'a'), true);
        ASSERT_EQ(str::icontains("abc", 'B'), true);
        ASSERT_EQ(str::icontains("abc", 'c'), true);
        ASSERT_EQ(str::icontains("abc", "AbC"), true);
    }
}

