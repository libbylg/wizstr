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

TEST(test_str, simplified_integer) {

    ASSERT_EQ(str::simplified_integer("123"), "123");
    ASSERT_EQ(str::simplified_integer("000123"), "123");
    ASSERT_EQ(str::simplified_integer("+0000123"), "+123");
    ASSERT_EQ(str::simplified_integer("-0000123"), "-123");
    ASSERT_EQ(str::simplified_integer("00000"), "0");
    ASSERT_EQ(str::simplified_integer("+0000"), "+0");
    ASSERT_EQ(str::simplified_integer("-0000"), "-0");

    ASSERT_EQ(str::simplified_integer("000  "), "0  ");
    ASSERT_EQ(str::simplified_integer("abc"), "abc");
    ASSERT_EQ(str::simplified_integer("  000123"), "  000123");
    ASSERT_EQ(str::simplified_integer("+abc"), "+abc");
    ASSERT_EQ(str::simplified_integer("+"), "+");
    ASSERT_EQ(str::simplified_integer(""), "");
}
