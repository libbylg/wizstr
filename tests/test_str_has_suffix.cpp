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

TEST(test_str, has_suffix) {
    SECTION("一般情况") {
        ASSERT_EQ(str::has_suffix("aaa", "baa"), false);
    }
    SECTION("无共同后缀") {
        ASSERT_EQ(str::has_suffix("aaa", "bbb"), false);
    }
    SECTION("完全相同") {
        ASSERT_EQ(str::has_suffix("aaa", "aaa"), true);
    }
    SECTION("部分为空") {
        ASSERT_EQ(str::has_suffix("", "aaa"), false);
        ASSERT_EQ(str::has_suffix("aaa", ""), true);
    }
    SECTION("空对空") {
        ASSERT_EQ(str::has_suffix("", ""), true);
    }
    SECTION("包含关系") {
        ASSERT_EQ(str::has_suffix("abc", "bc"), true);
        ASSERT_EQ(str::has_suffix("bc", "abc"), false);
    }
    SECTION("字符后缀") {
        ASSERT_EQ(str::has_suffix("HelloWorld", 'd'), true);
        ASSERT_EQ(str::has_suffix("HelloWorld", 'K'), false);
        ASSERT_EQ(str::has_suffix("", 'H'), false);
        ASSERT_EQ(str::has_suffix("", '\0'), false);
    }
}
