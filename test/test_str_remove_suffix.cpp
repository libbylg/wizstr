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

TEST(test_str, remove_suffix) {
    SECTION("一般情况") {
        ASSERT_EQ(str::remove_suffix("aaa", "baa"), "aaa");
    }
    SECTION("无共同后缀") {
        ASSERT_EQ(str::remove_suffix("aaa", "bbb"), "aaa");
    }
    SECTION("完全相同") {
        ASSERT_EQ(str::remove_suffix("aaa", "aaa"), "");
    }
    SECTION("部分为空") {
        ASSERT_EQ(str::remove_suffix("", "aaa"), "");
        ASSERT_EQ(str::remove_suffix("aaa", ""), "aaa");
    }
    SECTION("空对空") {
        ASSERT_EQ(str::remove_suffix("", ""), "");
    }
    SECTION("包含关系") {
        ASSERT_EQ(str::remove_suffix("abc", "bc"), "a");
        ASSERT_EQ(str::remove_suffix("bc", "abc"), "bc");
    }
    SECTION("字符后缀") {
        ASSERT_EQ(str::remove_suffix("HelloWorld", 'd'), "HelloWorl");
        ASSERT_EQ(str::remove_suffix("HelloWorld", 'K'), "HelloWorld");
        ASSERT_EQ(str::remove_suffix("", 'H'), "");
        ASSERT_EQ(str::remove_suffix("", '\0'), "");
    }
}
