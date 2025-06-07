/**
 * Copyright (c) 2021-2025  libbylg@126.com
 * str is licensed under Mulan PSL v2.
 * You can use this software according to the terms and conditions of the Mulan PSL v2.
 * You may obtain a copy of Mulan PSL v2 at:
 *          http://license.coscl.org.cn/MulanPSL2
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY OR
 * FIT FOR A PARTICULAR PURPOSE.
 * See the Mulan PSL v2 for more details.
 */
#include "testing.hpp"

#include "str.hpp"

TEST(test_str, drop_left) {
    SECTION("一般场景") {
        ASSERT_EQ(str::drop_left_view("3bc1233", 0), "3bc1233");
        ASSERT_EQ(str::drop_left_view("3bc1233", 3), "1233");
        ASSERT_EQ(str::drop_left_view("3bc1233", 7), "");
        ASSERT_EQ(str::drop_left_view("3bc1233", 8), "");
        ASSERT_EQ(str::drop_left_view("3bc1233", str::npos), "");
    }
    SECTION("空串") {
        ASSERT_EQ(str::drop_left_view("", 0), "");
        ASSERT_EQ(str::drop_left_view("", 3), "");
        ASSERT_EQ(str::drop_left_view("", str::npos), "");
    }
}

TEST(test_str, drop_right) {
    SECTION("一般场景") {
        ASSERT_EQ(str::drop_right_view("3bc1233", 0), "3bc1233");
        ASSERT_EQ(str::drop_right_view("3bc1233", 3), "3bc1");
        ASSERT_EQ(str::drop_right_view("3bc1233", 7), "");
        ASSERT_EQ(str::drop_right_view("3bc1233", 8), "");
        ASSERT_EQ(str::drop_right_view("3bc1233", str::npos), "");
    }
    SECTION("空串") {
        ASSERT_EQ(str::drop_right_view("", 0), "");
        ASSERT_EQ(str::drop_right_view("", 3), "");
        ASSERT_EQ(str::drop_right_view("", str::npos), "");
    }
}

TEST(test_str, drop_mid) {
    SECTION("一般场景") {
        ASSERT_EQ(str::drop_mid("3bc1233", 3, 0), "3bc1233");
        ASSERT_EQ(str::drop_mid("3bc1233", 2, 3), "3b33");
        ASSERT_EQ(str::drop_mid("3bc1233", 2, 5), "3b");
        ASSERT_EQ(str::drop_mid("3bc1233", 2, 6), "3b");
        ASSERT_EQ(str::drop_mid("3bc1233", 2, str::npos), "3b");
    }
    SECTION("起始位置") {
        ASSERT_EQ(str::drop_mid("3bc1233", 0, 2), "c1233");
        ASSERT_EQ(str::drop_mid("3bc1233", 2, 3), "3b33");
        ASSERT_EQ(str::drop_mid("3bc1234", 6, 1), "3bc123");
        ASSERT_EQ(str::drop_mid("3bc1234", 6, 2), "3bc123");
        ASSERT_EQ(str::drop_mid("3bc1234", 7, 1), "3bc1234");
        ASSERT_EQ(str::drop_mid("3bc1233", str::npos, 6), "3bc1233");
    }
    SECTION("空串") {
        ASSERT_EQ(str::drop_mid("", 0, 0), "");
        ASSERT_EQ(str::drop_mid("", 2, 0), "");
        ASSERT_EQ(str::drop_mid("", 3, str::npos), "");
    }
}

TEST(test_str, drop) {
    SECTION("一般场景") {
        ASSERT_EQ(str::drop("3bc1233", 3, 0), "3bc1233");
        ASSERT_EQ(str::drop("3bc1233", 2, 3), "3b33");
        ASSERT_EQ(str::drop("3bc1233", 2, -2), "31233");
        ASSERT_EQ(str::drop("3bc1233", 2, 5), "3b");
        ASSERT_EQ(str::drop("3bc1233", 2, -3), "1233");
        ASSERT_EQ(str::drop("3bc1233", 2, 6), "3b");
        ASSERT_EQ(str::drop("3bc1233", 2, -4), "1233");
    }
    SECTION("起始位置") {
        ASSERT_EQ(str::drop("3bc1233", 0, 2), "c1233");
        ASSERT_EQ(str::drop("3bc1233", 0, -2), "bc1233");
        ASSERT_EQ(str::drop("3bc1233", 0, 0), "3bc1233");
        ASSERT_EQ(str::drop("3bc1233", 6, -7), "");
        ASSERT_EQ(str::drop("3bc1234", 6, 1), "3bc123");
        ASSERT_EQ(str::drop("3bc1234", 7, -1), "3bc123");
        ASSERT_EQ(str::drop("3bc1234", 7, -2), "3bc12");
        ASSERT_EQ(str::drop("3bc1233", str::npos, 6), "3bc1233");
    }
    SECTION("空串") {
        ASSERT_EQ(str::drop("", 0, 0), "");
        ASSERT_EQ(str::drop("", 2, -2), "");
        ASSERT_EQ(str::drop("", 3, str::npos), "");
    }
}