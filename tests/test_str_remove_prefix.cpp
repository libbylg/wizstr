/**
 * Copyright (c) 2021-2025  libbylg@126.com
 * wizstr is licensed under Mulan PSL v2.
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

#if defined(WIZSTR_NAMESPACE)
using str = WIZSTR_NAMESPACE::str;
#endif

TEST(test_str, remove_prefix) {
    SECTION("一般情况") {
        ASSERT_EQ(str::remove_prefix("aac", "aa"), "c");
        ASSERT_EQ(str::remove_prefix("aaa", "aab"), "aaa");
    }
    SECTION("无共同前缀") {
        ASSERT_EQ(str::remove_prefix("aaa", "bb"), "aaa");
    }
    SECTION("完全相同") {
        ASSERT_EQ(str::remove_prefix("aaa", "aaa"), "");
    }
    SECTION("部分为空") {
        ASSERT_EQ(str::remove_prefix("", "aaa"), "");
        ASSERT_EQ(str::remove_prefix("aaa", ""), "aaa");
    }
    SECTION("空对空") {
        ASSERT_EQ(str::remove_prefix("", ""), "");
    }
    SECTION("包含关系") {
        ASSERT_EQ(str::remove_prefix("aac", "aa"), "c");
    }
    SECTION("空字符串找前缀") {
        ASSERT_EQ(str::remove_prefix("", "Hello"), "");
        ASSERT_EQ(str::remove_prefix("Hello", ""), "Hello");
        ASSERT_EQ(str::remove_prefix("", ""), "");
    }
    SECTION("字符前缀") {
        ASSERT_EQ(str::remove_prefix("HelloWorld", 'H'), "elloWorld");
        ASSERT_EQ(str::remove_prefix("HelloWorld", 'W'), "HelloWorld");
        ASSERT_EQ(str::remove_prefix("", 'H'), "");
        ASSERT_EQ(str::remove_prefix("", '\0'), "");
    }
}
