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

TEST(test_str, is_literal_false) {
    SECTION("标准值") {
        ASSERT_EQ(str::is_literal_false("1"), false);
        ASSERT_EQ(str::is_literal_false("0"), true);
        ASSERT_EQ(str::is_literal_false("on"), false);
        ASSERT_EQ(str::is_literal_false("off"), true);
        ASSERT_EQ(str::is_literal_false("ON"), false);
        ASSERT_EQ(str::is_literal_false("OFF"), true);
        ASSERT_EQ(str::is_literal_false("On"), false);
        ASSERT_EQ(str::is_literal_false("Off"), true);
        ASSERT_EQ(str::is_literal_false("Yes"), false);
        ASSERT_EQ(str::is_literal_false("No"), true);
        ASSERT_EQ(str::is_literal_false("yes"), false);
        ASSERT_EQ(str::is_literal_false("no"), true);
        ASSERT_EQ(str::is_literal_false("YES"), false);
        ASSERT_EQ(str::is_literal_false("NO"), true);
        ASSERT_EQ(str::is_literal_false("True"), false);
        ASSERT_EQ(str::is_literal_false("False"), true);
        ASSERT_EQ(str::is_literal_false("true"), false);
        ASSERT_EQ(str::is_literal_false("false"), true);
        ASSERT_EQ(str::is_literal_false("TRUE"), false);
        ASSERT_EQ(str::is_literal_false("FALSE"), true);
    }
    SECTION("空串") {
        ASSERT_EQ(str::is_literal_false(""), false);
    }
    SECTION("非标值") {
        ASSERT_EQ(str::is_literal_false("FalsE"), false);
        ASSERT_EQ(str::is_literal_false("tRUe"), false);
        ASSERT_EQ(str::is_literal_false("true "), false);
    }
    SECTION("超长值") {
        ASSERT_EQ(str::is_literal_false("hello-workd"), false);
    }
}
