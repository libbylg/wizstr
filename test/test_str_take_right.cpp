#include "testing.hpp"

#include "str.hpp"

TEST(test_str, take_right) {
    SECTION("一般场景") {
        ASSERT_EQ(str::take_right("3bc1233", 0), "");
        ASSERT_EQ(str::take_right("3bc1233", 3), "233");
        ASSERT_EQ(str::take_right("3bc1233", 7), "3bc1233");
        ASSERT_EQ(str::take_right("3bc1233", 8), "3bc1233");
        ASSERT_EQ(str::take_right("3bc1233", str::npos), "3bc1233");
    }
    SECTION("空串") {
        ASSERT_EQ(str::take_right("", 0), "");
        ASSERT_EQ(str::take_right("", 3), "");
        ASSERT_EQ(str::take_right("", str::npos), "");
    }
}
