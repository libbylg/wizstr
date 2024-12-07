#include "tester.hpp"

#include "str.hpp"
#include "view.hpp"

TEST(test_view, is_space) {
    ASSERT_EQ(view::is_space("\r\n\v\t "), true);
    ASSERT_EQ(view::is_space(""), false);
    ASSERT_EQ(view::is_space("123DEF"), false);
    ASSERT_EQ(view::is_space("\r\n\v\t #"), false);
}
