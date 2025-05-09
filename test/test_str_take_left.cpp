#include "testing.hpp"

#include "str.hpp"

TEST(test_str, take_left) {
    SECTION("一般场景") {
        ASSERT_EQ(str::take_left("3bc1233", 0), "");
        ASSERT_EQ(str::take_left("3bc1233", 3), "3bc");
        ASSERT_EQ(str::take_left("3bc1233", 7), "3bc1233");
        ASSERT_EQ(str::take_left("3bc1233", 8), "3bc1233");
        ASSERT_EQ(str::take_left("3bc1233", str::npos), "3bc1233");
    }
    SECTION("空串") {
        ASSERT_EQ(str::take_left("", 0), "");
        ASSERT_EQ(str::take_left("", 3), "");
        ASSERT_EQ(str::take_left("", str::npos), "");
    }
}
