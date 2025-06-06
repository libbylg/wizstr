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

TEST(test_str, is_capitalize) {
    SECTION("一般情况") {
        ASSERT_EQ(str::is_capitalize("A"), true);
        ASSERT_EQ(str::is_capitalize("This Is Hello World"), true);
        ASSERT_EQ(str::is_capitalize("this is hello world"), false);
        ASSERT_EQ(str::is_capitalize("THIS IS HELLO world"), true);
        ASSERT_EQ(str::is_capitalize("123 456 789"), false);
        ASSERT_EQ(str::is_capitalize("123A 456A 789A"), false);
        ASSERT_EQ(str::is_capitalize("0"), false);
        ASSERT_EQ(str::is_capitalize(""), false);
    }
    SECTION("空格") {
        ASSERT_EQ(str::is_capitalize(" \t\r\n\vA \t\r\n\v"), false);
        ASSERT_EQ(str::is_capitalize(" \t\r\n\v"), false);
    }
    SECTION("Extr") {
        ASSERT_EQ(str::is_capitalize("1"), false);
        ASSERT_EQ(str::is_capitalize(""), false);
        ASSERT_EQ(str::is_capitalize("Abc123DEF"), true);
        ASSERT_EQ(str::is_capitalize("abc123DEF"), false);
        ASSERT_EQ(str::is_capitalize("  A"), false);
        ASSERT_EQ(str::is_capitalize("A"), true);
    }
}
