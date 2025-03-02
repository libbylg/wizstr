#include "tester.hpp"

#include "str.hpp"
#include "view.hpp"

TEST(test_view, is_identifier) {
    ASSERT_EQ(view::is_identifier("0123456789"), false);
    ASSERT_EQ(view::is_identifier(""), false);
    ASSERT_EQ(view::is_identifier("123DEF"), false);
    ASSERT_EQ(view::is_identifier("ABC "), false);
    ASSERT_EQ(view::is_identifier("  ABC"), false);
    ASSERT_EQ(view::is_identifier("ABCDEF"), true);
    ASSERT_EQ(view::is_identifier("_"), true);
    ASSERT_EQ(view::is_identifier("_123"), true);
    ASSERT_EQ(view::is_identifier("_ABC"), true);
    ASSERT_EQ(view::is_identifier("XXX_HOME"), true);
}
