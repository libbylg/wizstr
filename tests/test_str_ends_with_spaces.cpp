/**
 * Copyright (c) 2021-2025  libbylg@126.com
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

#if defined(STR_NAMESPACE)
using str = STR_NAMESPACE::str;
#endif

TEST(test_str, ends_with_spaces) {
    SECTION("一般情况") {
        ASSERT_FALSE(str::ends_with_spaces("aaa"));
        ASSERT_TRUE(str::ends_with_spaces("aaa\r"));
        ASSERT_TRUE(str::ends_with_spaces("aaa     "));
        ASSERT_TRUE(str::ends_with_spaces("aaa\t\r"));
        ASSERT_TRUE(str::ends_with_spaces(" "));
    }
    SECTION("空串") {
        ASSERT_FALSE(str::ends_with_spaces(""));
    }
}