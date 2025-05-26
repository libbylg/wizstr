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

TEST(test_str, rawname_range) {
    SECTION("一般情况") {
        ASSERT_EQ(str::rawname_range("/aaa/bbb/ccc.txt"), str::range(9, 3));
        ASSERT_EQ(str::rawname_range("/aaa/bbb/ccc"), str::range(9, 3));
        ASSERT_EQ(str::rawname_range("/aaa/bbb/"), str::range(9, 0));
        ASSERT_EQ(str::rawname_range("/aaa/bbb/.ccc"), str::range(9, 4));
        ASSERT_EQ(str::rawname_range("/aaa/bbb/.ccc.xml"), str::range(9, 4));
        ASSERT_EQ(str::rawname_range("/"), str::range(1, 0));
        ASSERT_EQ(str::rawname_range("."), str::range(1, 0));
        ASSERT_EQ(str::rawname_range(".."), str::range(2, 0));
        ASSERT_EQ(str::rawname_range("a"), str::range(0, 1));
        ASSERT_EQ(str::rawname_range("a."), str::range(0, 1));
        ASSERT_EQ(str::rawname_range(".a"), str::range(0, 2));
        ASSERT_EQ(str::rawname_range(".abc.txt"), str::range(0, 4));
        ASSERT_EQ(str::rawname_range(""), str::range(0, 0));
    }
}
