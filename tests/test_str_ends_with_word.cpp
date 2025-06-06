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

TEST(test_str, ends_with_word) {
    SECTION("一般情况") {
        ASSERT_FALSE(str::ends_with_word("aaabbb", "bbb"));
        ASSERT_TRUE(str::ends_with_word("aaa", "aaa"));
        ASSERT_TRUE(str::ends_with_word("aaa bbb", "bbb"));
        ASSERT_TRUE(str::ends_with_word("\r\t\r\n \vaaa", "aaa"));
        ASSERT_TRUE(str::ends_with_word("aaa\r\t\r\n \v", "aaa"));
        ASSERT_FALSE(str::ends_with_word("  ", "  "));
        ASSERT_FALSE(str::ends_with_word("\r\t\r\n \v", ""));
        ASSERT_TRUE(str::ends_with_word("\r\t\r\n \va\r\t\r\n \vb", "b"));
     }
    SECTION("空串") {
        ASSERT_FALSE(str::ends_with_word("", ""));
        ASSERT_FALSE(str::ends_with_word("aaa", ""));
        ASSERT_FALSE(str::ends_with_word("", "aaa"));
        ASSERT_FALSE(str::ends_with_word("a a a", ""));
        ASSERT_FALSE(str::ends_with_word("  ", ""));
    }
}