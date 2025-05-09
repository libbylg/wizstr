#include "testing.hpp"

#include "str.hpp"

TEST(test_str, take_left_view) {
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
