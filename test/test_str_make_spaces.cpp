#include "testing.hpp"

#include "str.hpp"

TEST(test_str, make_spaces) {
    SECTION("简单字符串重复多次") {
        ASSERT_EQ(str::make_spaces(5), "     ");
        ASSERT_EQ(str::make_spaces(0), "");
    }
}
