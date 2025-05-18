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

TEST(test_str, is_title) {
    SECTION("一般情况") {
        ASSERT_EQ(str::is_title("A"), true);
        ASSERT_EQ(str::is_title("a"), false);
        ASSERT_EQ(str::is_title("0"), true);
        ASSERT_EQ(str::is_title("This"), true);
        ASSERT_EQ(str::is_title("this"), false);
        ASSERT_EQ(str::is_title("THIS"), false);
        ASSERT_EQ(str::is_title("123"), true);
        ASSERT_EQ(str::is_title("123A"), true);
        ASSERT_EQ(str::is_title("123AbcDef"), false);
        ASSERT_EQ(str::is_title("123Abc Def"), true);
        ASSERT_EQ(str::is_title("123Abc456Def"), true);
    }
    SECTION("多字段") {
        ASSERT_EQ(str::is_title(""), false);
        ASSERT_EQ(str::is_title("123456"), true);
        ASSERT_EQ(str::is_title("Abc 123DEF"), false);
        ASSERT_EQ(str::is_title("Abc 123dEF"), false);
        ASSERT_EQ(str::is_title(" abc 123DEF"), false);
        ASSERT_EQ(str::is_title(" Abc Def 123"), true);
        ASSERT_EQ(str::is_title("  A"), true);
        ASSERT_EQ(str::is_title("  a"), false);
    }
    SECTION("空格") {
        ASSERT_EQ(str::is_title(" \t\r\n\vA \t\r\n\v"), true);
        ASSERT_EQ(str::is_title(" \t\r\n\v"), true);
    }
}
