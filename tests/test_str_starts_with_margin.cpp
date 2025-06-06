/**
 * Copyright (c) 2021-2024 libbylg@126.com
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

TEST(test_str, starts_with_margin) {
    SECTION("一般情况") {
        ASSERT_TRUE(str::starts_with_margin("|aaa", '|'));
        ASSERT_TRUE(str::starts_with_margin("\r|aaa", '|'));
        ASSERT_TRUE(str::starts_with_margin("     |aaa", '|'));
        ASSERT_TRUE(str::starts_with_margin("\t\r|aaa", '|'));
        ASSERT_TRUE(str::starts_with_margin(" |", '|'));
        ASSERT_FALSE(str::starts_with_margin(" ", '*'));
        ASSERT_FALSE(str::starts_with_margin(" ", ' '));
    }
    SECTION("空串") {
        ASSERT_FALSE(str::starts_with_margin("", '|'));
    }
}