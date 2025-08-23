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

TEST(test_str, to_upper) {
    SECTION("一般情况") {
        ASSERT_EQ(str::to_upper("  AabbCC中华人民共和国"), "  AABBCC中华人民共和国");
        ASSERT_EQ(str::to_upper("中华人民共和国"), "中华人民共和国");
        ASSERT_EQ(str::to_upper("0123456789*&^%#$@"), "0123456789*&^%#$@");
    }
    SECTION("空串") {
        ASSERT_EQ(str::to_upper(""), "");
    }
    SECTION("全部是大写或者小写") {
        ASSERT_EQ(str::to_upper("ABCDEFGHIJK"), "ABCDEFGHIJK");
        ASSERT_EQ(str::to_upper("abcdefghijk"), "ABCDEFGHIJK");
    }
}
