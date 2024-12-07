#include "tester.hpp"

#include "str.hpp"
#include "view.hpp"

TEST(test_view, take_left) {
    SECTION("一般场景") {
        ASSERT_EQ(view::take_left("3bc1233", 0), "");
        ASSERT_EQ(view::take_left("3bc1233", 3), "3bc");
        ASSERT_EQ(view::take_left("3bc1233", 7), "3bc1233");
        ASSERT_EQ(view::take_left("3bc1233", 8), "3bc1233");
        ASSERT_EQ(view::take_left("3bc1233", view::npos), "3bc1233");
    }
    SECTION("空串") {
        ASSERT_EQ(view::take_left("", 0), "");
        ASSERT_EQ(view::take_left("", 3), "");
        ASSERT_EQ(view::take_left("", view::npos), "");
    }
}

TEST(test_view, take_right) {
    SECTION("一般场景") {
        ASSERT_EQ(view::take_right("3bc1233", 0), "");
        ASSERT_EQ(view::take_right("3bc1233", 3), "233");
        ASSERT_EQ(view::take_right("3bc1233", 7), "3bc1233");
        ASSERT_EQ(view::take_right("3bc1233", 8), "3bc1233");
        ASSERT_EQ(view::take_right("3bc1233", view::npos), "3bc1233");
    }
    SECTION("空串") {
        ASSERT_EQ(view::take_right("", 0), "");
        ASSERT_EQ(view::take_right("", 3), "");
        ASSERT_EQ(view::take_right("", view::npos), "");
    }
}

TEST(test_view, take_mid) {
    SECTION("一般场景") {
        ASSERT_EQ(view::take_mid("3bc1233", 3, 0), "");
        ASSERT_EQ(view::take_mid("3bc1233", 2, 3), "c12");
        ASSERT_EQ(view::take_mid("3bc1233", 2, 5), "c1233");
        ASSERT_EQ(view::take_mid("3bc1233", 2, 6), "c1233");
        ASSERT_EQ(view::take_mid("3bc1233", 2, view::npos), "c1233");
    }
    SECTION("起始位置") {
        ASSERT_EQ(view::take_mid("3bc1233", 0, 2), "3b");
        ASSERT_EQ(view::take_mid("3bc1233", 2, 3), "c12");
        ASSERT_EQ(view::take_mid("3bc1234", 6, 1), "4");
        ASSERT_EQ(view::take_mid("3bc1234", 6, 2), "4");
        ASSERT_EQ(view::take_mid("3bc1234", 7, 1), "");
        ASSERT_EQ(view::take_mid("3bc1233", view::npos, 6), "");
    }
    SECTION("空串") {
        ASSERT_EQ(view::take_mid("", 0, 0), "");
        ASSERT_EQ(view::take_mid("", 2, 0), "");
        ASSERT_EQ(view::take_mid("", 3, view::npos), "");
    }
}


TEST(test_view, take) {
    SECTION("一般场景") {
        ASSERT_EQ(view::take("3bc1233", 3, 0), "");
        ASSERT_EQ(view::take("3bc1233", 2, 3), "c12");
        ASSERT_EQ(view::take("3bc1233", 2, -2), "bc");
        ASSERT_EQ(view::take("3bc1233", 2, 5), "c1233");
        ASSERT_EQ(view::take("3bc1233", 2, -3), "3bc");
        ASSERT_EQ(view::take("3bc1233", 2, 6), "c1233");
        ASSERT_EQ(view::take("3bc1233", 2, -4), "3bc");
    }
    SECTION("起始位置") {
        ASSERT_EQ(view::take("3bc1233", 0, 2), "3b");
        ASSERT_EQ(view::take("3bc1233", 0, -2), "3");
        ASSERT_EQ(view::take("3bc1233", 0, 0), "");
        ASSERT_EQ(view::take("3bc1233", 6, -7), "3bc1233");
        ASSERT_EQ(view::take("3bc1234", 6, 1), "4");
        ASSERT_EQ(view::take("3bc1234", 7, -1), "4");
        ASSERT_EQ(view::take("3bc1234", 7, -2), "34");
        ASSERT_EQ(view::take("3bc1233", view::npos, 6), "");
    }
    SECTION("空串") {
        ASSERT_EQ(view::take("", 0, 0), "");
        ASSERT_EQ(view::take("", 2, -2), "");
        ASSERT_EQ(view::take("", 3, view::npos), "");
    }
}