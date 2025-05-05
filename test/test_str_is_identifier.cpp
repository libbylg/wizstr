#include "testing.hpp"

#include "str.hpp"


TEST(test_str, is_identifier) {
    ASSERT_EQ(str::is_identifier("0123456789"), false);
    ASSERT_EQ(str::is_identifier(""), false);
    ASSERT_EQ(str::is_identifier("123DEF"), false);
    ASSERT_EQ(str::is_identifier("ABC "), false);
    ASSERT_EQ(str::is_identifier("  ABC"), false);
    ASSERT_EQ(str::is_identifier("ABCDEF"), true);
    ASSERT_EQ(str::is_identifier("_"), true);
    ASSERT_EQ(str::is_identifier("A"), true);
    ASSERT_EQ(str::is_identifier("_123"), true);
    ASSERT_EQ(str::is_identifier("_ABC"), true);
    ASSERT_EQ(str::is_identifier("XXX_HOME"), true);
    ASSERT_EQ(str::is_identifier("@123"), false);
}
