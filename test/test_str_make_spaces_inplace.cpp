#include "testing.hpp"

#include "str.hpp"

TEST(test_str, make_spaces_inplace) {
    std::string s;
    SECTION("简单字符串重复多次") {
        ASSERT_EQ(str::make_spaces_inplace(s, 5), "     ");
        ASSERT_EQ(str::make_spaces_inplace(s, 0), "");
        ASSERT_EQ(str::make_spaces_inplace(s = "ABCDEFGHIJKLMNOPQRETUVWXYZ", 5), "     ");
        ASSERT_EQ(str::make_spaces_inplace(s = "ABCDEFGHIJKLMNOPQRETUVWXYZ", 0), "");
    }
}
