#include "testing.hpp"

#include "str.hpp"

TEST(test_str, take_view) {
    SECTION("一般场景") {
        ASSERT_EQ(str::take_view("3bc1233", 3, 0), "");
        ASSERT_EQ(str::take_view("3bc1233", 2, 3), "c12");
        ASSERT_EQ(str::take_view("3bc1233", 2, -2), "bc");
        ASSERT_EQ(str::take_view("3bc1233", 2, 5), "c1233");
        ASSERT_EQ(str::take_view("3bc1233", 2, -3), "3bc");
        ASSERT_EQ(str::take_view("3bc1233", 2, 6), "c1233");
        ASSERT_EQ(str::take_view("3bc1233", 2, -4), "3bc");
    }
    SECTION("起始位置") {
        ASSERT_EQ(str::take_view("3bc1233", 0, 2), "3b");
        ASSERT_EQ(str::take_view("3bc1233", 0, -2), "3");
        ASSERT_EQ(str::take_view("3bc1233", 0, 0), "");
        ASSERT_EQ(str::take_view("3bc1233", 6, -7), "3bc1233");
        ASSERT_EQ(str::take_view("3bc1234", 6, 1), "4");
        ASSERT_EQ(str::take_view("3bc1234", 7, -1), "4");
        ASSERT_EQ(str::take_view("3bc1234", 7, -2), "34");
        ASSERT_EQ(str::take_view("3bc1233", str::npos, 6), "");
    }
    SECTION("空串") {
        ASSERT_EQ(str::take_view("", 0, 0), "");
        ASSERT_EQ(str::take_view("", 2, -2), "");
        ASSERT_EQ(str::take_view("", 3, str::npos), "");
    }
    SECTION("无offset场景") {
        ASSERT_EQ(str::take_view("3bc1233", 0 ), "3bc1233");
        ASSERT_EQ(str::take_view("3bc1233", 3), "1233");
        ASSERT_EQ(str::take_view("3bc1239", 6), "9");
        ASSERT_EQ(str::take_view("3bc1234", 7), "");
        ASSERT_EQ(str::take_view("3bc1234", str::npos), "");
        ASSERT_EQ(str::take_view("", 0), "");
        ASSERT_EQ(str::take_view("", 1), "");
        ASSERT_EQ(str::take_view("", str::npos), "");
    }
}