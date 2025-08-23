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

TEST(test_str, icompare) {
    SECTION("同长字符串对比") {
        ASSERT_EQ(str::icompare("ABCdef", "abcdef"), 0);
        ASSERT_EQ(str::icompare("ABCDEF", "ABCDEF"), 0);
        ASSERT_EQ(str::icompare("abcdef", "ABCDEF"), 0);
        ASSERT_EQ(str::icompare("01234$%#$%#$^", "01234$%#$%#$^"), 0);
        ASSERT_TRUE(str::icompare("abcdef", "ABC123") > 0);
    }
    SECTION("长短不一的对比") {
        ASSERT_TRUE(str::icompare("abc", "ABCdef") < 0);
        ASSERT_TRUE(str::icompare("abm", "ABCdef") > 0);
        ASSERT_TRUE(str::icompare("ABCdef", "abc") > 0);
        ASSERT_TRUE(str::icompare("ABadef", "abc") < 0);
    }
    SECTION("空串") {
        ASSERT_EQ(str::icompare("", ""), 0);
        ASSERT_TRUE(str::icompare("", "abc") < 0);
        ASSERT_TRUE(str::icompare("abc", "") > 0);
    }
    SECTION("指定最大对比长度") {
        ASSERT_EQ(str::icompare("", "", 0), 0);
        ASSERT_TRUE(str::icompare("", "abc", 1) < 0);
        ASSERT_TRUE(str::icompare("abc", "", 1) > 0);
        ASSERT_EQ(str::icompare("abc", "", 0), 0);
        ASSERT_EQ(str::icompare("abc", "ABCdef", 3), 0);
        ASSERT_TRUE(str::icompare("ABCdef", "abc", 4) > 0);
        ASSERT_EQ(str::icompare("ABCdef", "abcDEF", 6), 0);
        ASSERT_EQ(str::icompare("ABCdef", "abcDEF", 7), 0);
        ASSERT_EQ(str::icompare("ABCdef", "abcDEF", str::npos), 0);
    }
}
