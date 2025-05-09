#include "testing.hpp"

#include "str.hpp"

TEST(test_str, take_range_inplace) {
    std::string s;
    SECTION("一般场景") {
        ASSERT_EQ(str::take_range_inplace(s = "3bc1233", str::range(3, 0)), "");
        ASSERT_EQ(str::take_range_inplace(s = "3bc1233", str::range(2, 3)), "c12");
        ASSERT_EQ(str::take_range_inplace(s = "3bc1233", str::range(2, 5)), "c1233");
        ASSERT_EQ(str::take_range_inplace(s = "3bc1233", str::range(2, 6)), "c1233");
        ASSERT_EQ(str::take_range_inplace(s = "3bc1233", str::range(2, str::npos)), "c1233");
    }
    SECTION("起始位置") {
        ASSERT_EQ(str::take_range_inplace(s = "3bc1233", str::range(0, 2)), "3b");
        ASSERT_EQ(str::take_range_inplace(s = "3bc1233", str::range(2, 3)), "c12");
        ASSERT_EQ(str::take_range_inplace(s = "3bc1234", str::range(6, 1)), "4");
        ASSERT_EQ(str::take_range_inplace(s = "3bc1234", str::range(6, 2)), "4");
        ASSERT_EQ(str::take_range_inplace(s = "3bc1234", str::range(7, 1)), "");
        ASSERT_EQ(str::take_range_inplace(s = "3bc1233", str::range(str::npos, 6)), "");
    }
    SECTION("空串") {
        ASSERT_EQ(str::take_range_inplace(s = "", str::range(0, 0)), "");
        ASSERT_EQ(str::take_range_inplace(s = "", str::range(2, 0)), "");
        ASSERT_EQ(str::take_range_inplace(s = "", str::range(3, str::npos)), "");
    }
}
