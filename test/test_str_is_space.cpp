#include "testing.hpp"

#include "str.hpp"


TEST(test_str, is_space) {
    ASSERT_EQ(str::is_space("\r\n\v\t "), true);
    ASSERT_EQ(str::is_space(""), false);
    ASSERT_EQ(str::is_space("123DEF"), false);
    ASSERT_EQ(str::is_space("\r\n\v\t #"), false);
}
