#include "testing.hpp"

#include "str.hpp"

TEST(test_str, drop_left_view) {
    SECTION("一般场景") {
        ASSERT_EQ(str::drop_left_view("3bc1233", 0), "3bc1233");
        ASSERT_EQ(str::drop_left_view("3bc1233", 3), "1233");
        ASSERT_EQ(str::drop_left_view("3bc1233", 7), "");
        ASSERT_EQ(str::drop_left_view("3bc1233", 8), "");
        ASSERT_EQ(str::drop_left_view("3bc1233", str::npos), "");
    }
    SECTION("空串") {
        ASSERT_EQ(str::drop_left_view("", 0), "");
        ASSERT_EQ(str::drop_left_view("", 3), "");
        ASSERT_EQ(str::drop_left_view("", str::npos), "");
    }
}
