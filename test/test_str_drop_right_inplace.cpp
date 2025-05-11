#include "testing.hpp"

#include "str.hpp"

TEST(test_str, drop_right_inplace) {
    std::string s;
    SECTION("一般场景") {
        ASSERT_EQ(str::drop_right_inplace(s = "3bc1233", 0), "3bc1233");
        ASSERT_EQ(str::drop_right_inplace(s = "3bc1233", 3), "3bc1");
        ASSERT_EQ(str::drop_right_inplace(s = "3bc1233", 7), "");
        ASSERT_EQ(str::drop_right_inplace(s = "3bc1233", 8), "");
        ASSERT_EQ(str::drop_right_inplace(s = "3bc1233", str::npos), "");
    }
    SECTION("空串") {
        ASSERT_EQ(str::drop_right_inplace(s = "", 0), "");
        ASSERT_EQ(str::drop_right_inplace(s = "", 3), "");
        ASSERT_EQ(str::drop_right_inplace(s = "", str::npos), "");
    }
}
