#include "testing.hpp"

#include "str.hpp"

TEST(test_str, drop) {
    SECTION("一般场景") {
        ASSERT_EQ(str::drop("3bc1233", 3, 0), "3bc1233");
        ASSERT_EQ(str::drop("3bc1233", 2, 3), "3b33");
        ASSERT_EQ(str::drop("3bc1233", 2, -2), "31233");
        ASSERT_EQ(str::drop("3bc1233", 2, 5), "3b");
        ASSERT_EQ(str::drop("3bc1233", 2, -3), "1233");
        ASSERT_EQ(str::drop("3bc1233", 2, 6), "3b");
        ASSERT_EQ(str::drop("3bc1233", 2, -4), "1233");
    }
    SECTION("起始位置") {
        ASSERT_EQ(str::drop("3bc1233", 0, 2), "c1233");
        ASSERT_EQ(str::drop("3bc1233", 0, -2), "bc1233");
        ASSERT_EQ(str::drop("3bc1233", 0, 0), "3bc1233");
        ASSERT_EQ(str::drop("3bc1233", 6, -7), "");
        ASSERT_EQ(str::drop("3bc1234", 6, 1), "3bc123");
        ASSERT_EQ(str::drop("3bc1234", 7, -1), "3bc123");
        ASSERT_EQ(str::drop("3bc1234", 7, -2), "3bc12");
        ASSERT_EQ(str::drop("3bc1233", str::npos, 6), "3bc1233");
    }
    SECTION("空串") {
        ASSERT_EQ(str::drop("", 0, 0), "");
        ASSERT_EQ(str::drop("", 2, -2), "");
        ASSERT_EQ(str::drop("", 3, str::npos), "");
    }
}