#include "tester.hpp"

#include "str.hpp"
#include "view.hpp"

TEST(test_view, drop_left) {
    SECTION("一般场景") {
        ASSERT_EQ(view::drop_left("3bc1233", 0), "3bc1233");
        ASSERT_EQ(view::drop_left("3bc1233", 3), "1233");
        ASSERT_EQ(view::drop_left("3bc1233", 7), "");
        ASSERT_EQ(view::drop_left("3bc1233", 8), "");
        ASSERT_EQ(view::drop_left("3bc1233", view::npos), "");
    }
    SECTION("空串") {
        ASSERT_EQ(view::drop_left("", 0), "");
        ASSERT_EQ(view::drop_left("", 3), "");
        ASSERT_EQ(view::drop_left("", view::npos), "");
    }
}

TEST(test_view, drop_right) {
    SECTION("一般场景") {
        ASSERT_EQ(view::drop_right("3bc1233", 0), "3bc1233");
        ASSERT_EQ(view::drop_right("3bc1233", 3), "3bc1");
        ASSERT_EQ(view::drop_right("3bc1233", 7), "");
        ASSERT_EQ(view::drop_right("3bc1233", 8), "");
        ASSERT_EQ(view::drop_right("3bc1233", view::npos), "");
    }
    SECTION("空串") {
        ASSERT_EQ(view::drop_right("", 0), "");
        ASSERT_EQ(view::drop_right("", 3), "");
        ASSERT_EQ(view::drop_right("", view::npos), "");
    }
}

TEST(test_view, drop_mid) {
    SECTION("一般场景") {
        ASSERT_EQ(view::drop_mid("3bc1233", 3, 0), "3bc1233");
        ASSERT_EQ(view::drop_mid("3bc1233", 2, 3), "3b33");
        ASSERT_EQ(view::drop_mid("3bc1233", 2, 5), "3b");
        ASSERT_EQ(view::drop_mid("3bc1233", 2, 6), "3b");
        ASSERT_EQ(view::drop_mid("3bc1233", 2, view::npos), "3b");
    }
    SECTION("起始位置") {
        ASSERT_EQ(view::drop_mid("3bc1233", 0, 2), "c1233");
        ASSERT_EQ(view::drop_mid("3bc1233", 2, 3), "3b33");
        ASSERT_EQ(view::drop_mid("3bc1234", 6, 1), "3bc123");
        ASSERT_EQ(view::drop_mid("3bc1234", 6, 2), "3bc123");
        ASSERT_EQ(view::drop_mid("3bc1234", 7, 1), "3bc1234");
        ASSERT_EQ(view::drop_mid("3bc1233", view::npos, 6), "3bc1233");
    }
    SECTION("空串") {
        ASSERT_EQ(view::drop_mid("", 0, 0), "");
        ASSERT_EQ(view::drop_mid("", 2, 0), "");
        ASSERT_EQ(view::drop_mid("", 3, view::npos), "");
    }
}


TEST(test_view, drop) {
    SECTION("一般场景") {
        ASSERT_EQ(view::drop("3bc1233", 3, 0), "3bc1233");
        ASSERT_EQ(view::drop("3bc1233", 2, 3), "3b33");
        ASSERT_EQ(view::drop("3bc1233", 2, -2), "31233");
        ASSERT_EQ(view::drop("3bc1233", 2, 5), "3b");
        ASSERT_EQ(view::drop("3bc1233", 2, -3), "1233");
        ASSERT_EQ(view::drop("3bc1233", 2, 6), "3b");
        ASSERT_EQ(view::drop("3bc1233", 2, -4), "1233");
    }
    SECTION("起始位置") {
        ASSERT_EQ(view::drop("3bc1233", 0, 2), "c1233");
        ASSERT_EQ(view::drop("3bc1233", 0, -2), "bc1233");
        ASSERT_EQ(view::drop("3bc1233", 0, 0), "3bc1233");
        ASSERT_EQ(view::drop("3bc1233", 6, -7), "");
        ASSERT_EQ(view::drop("3bc1234", 6, 1), "3bc123");
        ASSERT_EQ(view::drop("3bc1234", 7, -1), "3bc123");
        ASSERT_EQ(view::drop("3bc1234", 7, -2), "3bc12");
        ASSERT_EQ(view::drop("3bc1233", view::npos, 6), "3bc1233");
    }
    SECTION("空串") {
        ASSERT_EQ(view::drop("", 0, 0), "");
        ASSERT_EQ(view::drop("", 2, -2), "");
        ASSERT_EQ(view::drop("", 3, view::npos), "");
    }
}