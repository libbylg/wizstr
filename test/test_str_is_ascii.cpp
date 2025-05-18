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

TEST(test_str, is_ascii) {
    ASSERT_EQ(str::is_ascii("0123456789ABCDEF#$%\r\n\t\v "), true);
    ASSERT_EQ(str::is_ascii("ABCDEF"), true);
    ASSERT_EQ(str::is_ascii("123DEF"), true);
    ASSERT_EQ(str::is_ascii(std::string_view{"\0\0", 2}), true);
    ASSERT_EQ(str::is_ascii("123你好DEF"), false);
    ASSERT_EQ(str::is_ascii(""), false);
}
