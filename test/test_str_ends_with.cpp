/**
 * Copyright (c) 2021-2024 libbylg@126.com
 * tiny is licensed under Mulan PSL v2.
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

TEST(test_str, ends_with) {
    SECTION("一般情况") {
        ASSERT_EQ(str::ends_with("aaa", "baa"), false);
    }
    SECTION("无共同后缀") {
        ASSERT_EQ(str::ends_with("aaa", "bbb"), false);
    }
    SECTION("完全相同") {
        ASSERT_EQ(str::ends_with("aaa", "aaa"), true);
    }
    SECTION("部分为空") {
        ASSERT_EQ(str::ends_with("", "aaa"), false);
        ASSERT_EQ(str::ends_with("aaa", ""), true);
    }
    SECTION("空对空") {
        ASSERT_EQ(str::ends_with("", ""), true);
    }
    SECTION("包含关系") {
        ASSERT_EQ(str::ends_with("abc", "bc"), true);
        ASSERT_EQ(str::ends_with("bc", "abc"), false);
    }
    SECTION("字符后缀") {
        ASSERT_EQ(str::ends_with("HelloWorld", 'd'), true);
        ASSERT_EQ(str::ends_with("HelloWorld", 'K'), false);
        ASSERT_EQ(str::ends_with("", 'H'), false);
        ASSERT_EQ(str::ends_with("", '\0'), false);
    }
}
