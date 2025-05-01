#include "testing.hpp"

#include "str.hpp"


TEST(test_str, take_left) {
    SECTION("一般场景") {
        ASSERT_EQ(str::take_left_view("3bc1233", 0), "");
        ASSERT_EQ(str::take_left_view("3bc1233", 3), "3bc");
        ASSERT_EQ(str::take_left_view("3bc1233", 7), "3bc1233");
        ASSERT_EQ(str::take_left_view("3bc1233", 8), "3bc1233");
        ASSERT_EQ(str::take_left_view("3bc1233", str::npos), "3bc1233");
    }
    SECTION("空串") {
        ASSERT_EQ(str::take_left_view("", 0), "");
        ASSERT_EQ(str::take_left_view("", 3), "");
        ASSERT_EQ(str::take_left_view("", str::npos), "");
    }
}

TEST(test_str, take_right) {
    SECTION("一般场景") {
        ASSERT_EQ(str::take_right_view("3bc1233", 0), "");
        ASSERT_EQ(str::take_right_view("3bc1233", 3), "233");
        ASSERT_EQ(str::take_right_view("3bc1233", 7), "3bc1233");
        ASSERT_EQ(str::take_right_view("3bc1233", 8), "3bc1233");
        ASSERT_EQ(str::take_right_view("3bc1233", str::npos), "3bc1233");
    }
    SECTION("空串") {
        ASSERT_EQ(str::take_right_view("", 0), "");
        ASSERT_EQ(str::take_right_view("", 3), "");
        ASSERT_EQ(str::take_right_view("", str::npos), "");
    }
}

TEST(test_str, take_mid) {
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


TEST(test_str, take) {
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
}