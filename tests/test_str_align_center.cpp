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

TEST(test_str, align_center) {
    SECTION("一般情况：字符串居中对齐对齐，两边填充") {
        ASSERT_EQ(str::align_center("HelloWorld", 13, '*'), "*HelloWorld**");
        ASSERT_EQ(str::align_center("HelloWorld", 14, '*'), "**HelloWorld**");
        ASSERT_EQ(str::align_center("Hello", 13, '*'), "****Hello****");
        ASSERT_EQ(str::align_center("Hello", 14, '*'), "****Hello*****");
    }

    SECTION("宽度和原字符串相同：字符串不变") {
        ASSERT_EQ(str::align_center("HelloWorld", 10, '*'), "HelloWorld");
    }

    SECTION("宽度太窄时保持原样") {
        ASSERT_EQ(str::align_center("HelloWorld", 9, '*'), "HelloWorld");
    }

    SECTION("空字符串") {
        ASSERT_EQ(str::align_center("", 9, '*'), "*********");
    }

    SECTION("对齐宽度为0") {
        ASSERT_EQ(str::align_center("HelloWorld", 0, '*'), "HelloWorld");
        ASSERT_EQ(str::align_center("", 0, '*'), "");
    }
}
