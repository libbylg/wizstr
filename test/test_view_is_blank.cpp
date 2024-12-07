#include "tester.hpp"

#include "str.hpp"
#include "view.hpp"

TEST(test_view, is_blank) {
    ASSERT_EQ(view::is_blank(" \t\t "), true);
    ASSERT_EQ(view::is_blank("\r\n\v\t "), false);
    ASSERT_EQ(view::is_blank(""), false);
    ASSERT_EQ(view::is_blank("XYZ#"), false);
    ASSERT_EQ(view::is_blank("\t  XYZ#"), false);
    ASSERT_EQ(view::is_blank("XYZ#\t  "), false);
}
