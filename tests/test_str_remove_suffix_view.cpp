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

TEST(test_str, remove_suffix_view) {
    SECTION("一般情况") {
        ASSERT_EQ(str::remove_suffix_view("aaa", "baa"), "aaa");
    }
    SECTION("无共同后缀") {
        ASSERT_EQ(str::remove_suffix_view("aaa", "bbb"), "aaa");
    }
    SECTION("完全相同") {
        ASSERT_EQ(str::remove_suffix_view("aaa", "aaa"), "");
    }
    SECTION("部分为空") {
        ASSERT_EQ(str::remove_suffix_view("", "aaa"), "");
        ASSERT_EQ(str::remove_suffix_view("aaa", ""), "aaa");
    }
    SECTION("空对空") {
        ASSERT_EQ(str::remove_suffix_view("", ""), "");
    }
    SECTION("包含关系") {
        ASSERT_EQ(str::remove_suffix_view("abc", "bc"), "a");
        ASSERT_EQ(str::remove_suffix_view("bc", "abc"), "bc");
    }
    SECTION("字符后缀") {
        ASSERT_EQ(str::remove_suffix_view("HelloWorld", 'd'), "HelloWorl");
        ASSERT_EQ(str::remove_suffix_view("HelloWorld", 'K'), "HelloWorld");
        ASSERT_EQ(str::remove_suffix_view("", 'H'), "");
        ASSERT_EQ(str::remove_suffix_view("", '\0'), "");
    }
}
