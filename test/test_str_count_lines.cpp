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

#include <array>

TEST(test_str, count_lines) {
    SECTION("一般场景:CR+LF") {
        ASSERT_EQ(str::count_lines("\r\n\t A\r\n\t B\r\n\t C\r\n\t "), 5);
    }
    SECTION("一般场景:CR") {
        ASSERT_EQ(str::count_lines("\r\t A\r\t B\r\t C\r\t "), 5);
    }
    SECTION("一般场景:空串") {
        ASSERT_EQ(str::count_lines(""), 0);
    }
    SECTION("一般场景:无换行") {
        ASSERT_EQ(str::count_lines("abcd"), 1);
    }
    SECTION("一般场景:提前结束") {
        ASSERT_EQ(str::count_lines("a\rb\r\nc\rd\n"), 4);
    }
    SECTION("一般场景:不要结束符") {
        ASSERT_EQ(str::count_lines("a\rb\r\nc\rd"), 4);
    }
}