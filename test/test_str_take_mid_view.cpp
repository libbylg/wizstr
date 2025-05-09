#include "testing.hpp"

#include "str.hpp"


TEST(test_str, take_mid_view) {
    SECTION("一般场景") {
        ASSERT_EQ(str::take_mid_view("3bc1233", 3, 0), "");
        ASSERT_EQ(str::take_mid_view("3bc1233", 2, 3), "c12");
        ASSERT_EQ(str::take_mid_view("3bc1233", 2, 5), "c1233");
        ASSERT_EQ(str::take_mid_view("3bc1233", 2, 6), "c1233");
        ASSERT_EQ(str::take_mid_view("3bc1233", 2, str::npos), "c1233");
    }
    SECTION("起始位置") {
        ASSERT_EQ(str::take_mid_view("3bc1233", 0, 2), "3b");
        ASSERT_EQ(str::take_mid_view("3bc1233", 2, 3), "c12");
        ASSERT_EQ(str::take_mid_view("3bc1234", 6, 1), "4");
        ASSERT_EQ(str::take_mid_view("3bc1234", 6, 2), "4");
        ASSERT_EQ(str::take_mid_view("3bc1234", 7, 1), "");
        ASSERT_EQ(str::take_mid_view("3bc1233", str::npos, 6), "");
    }
    SECTION("空串") {
        ASSERT_EQ(str::take_mid_view("", 0, 0), "");
        ASSERT_EQ(str::take_mid_view("", 2, 0), "");
        ASSERT_EQ(str::take_mid_view("", 3, str::npos), "");
    }
}

