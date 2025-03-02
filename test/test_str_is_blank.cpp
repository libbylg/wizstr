#include "tester.hpp"

#include "str.hpp"


TEST(test_str, is_blank) {
    ASSERT_EQ(str::is_blank(" \t\t "), true);
    ASSERT_EQ(str::is_blank("\r\n\v\t "), false);
    ASSERT_EQ(str::is_blank(""), false);
    ASSERT_EQ(str::is_blank("XYZ#"), false);
    ASSERT_EQ(str::is_blank("\t  XYZ#"), false);
    ASSERT_EQ(str::is_blank("XYZ#\t  "), false);
}
