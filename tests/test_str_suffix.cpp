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

TEST(test_str, suffix) {
    SECTION("一般情况") {
        ASSERT_EQ(str::suffix("aaa", "baa"), 2);
    }
    SECTION("无共同后缀") {
        ASSERT_EQ(str::suffix("aaa", "bbb"), 0);
    }
    SECTION("完全相同") {
        ASSERT_EQ(str::suffix("aaa", "aaa"), 3);
    }
    SECTION("部分为空") {
        ASSERT_EQ(str::suffix("", "aaa"), 0);
        ASSERT_EQ(str::suffix("aaa", ""), 0);
    }
    SECTION("空对空") {
        ASSERT_EQ(str::suffix("", ""), 0);
    }
    SECTION("包含关系") {
        ASSERT_EQ(str::suffix("aaa", "aa"), 2);
        ASSERT_EQ(str::suffix("aa", "aaa"), 2);
    }
}
