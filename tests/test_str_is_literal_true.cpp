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

TEST(test_str, is_literal_true) {
    SECTION("标准值") {
        ASSERT_EQ(str::is_literal_true("1"), true);
        ASSERT_EQ(str::is_literal_true("0"), false);
        ASSERT_EQ(str::is_literal_true("on"), true);
        ASSERT_EQ(str::is_literal_true("off"), false);
        ASSERT_EQ(str::is_literal_true("ON"), true);
        ASSERT_EQ(str::is_literal_true("OFF"), false);
        ASSERT_EQ(str::is_literal_true("On"), true);
        ASSERT_EQ(str::is_literal_true("Off"), false);
        ASSERT_EQ(str::is_literal_true("Yes"), true);
        ASSERT_EQ(str::is_literal_true("No"), false);
        ASSERT_EQ(str::is_literal_true("yes"), true);
        ASSERT_EQ(str::is_literal_true("no"), false);
        ASSERT_EQ(str::is_literal_true("YES"), true);
        ASSERT_EQ(str::is_literal_true("NO"), false);
        ASSERT_EQ(str::is_literal_true("True"), true);
        ASSERT_EQ(str::is_literal_true("False"), false);
        ASSERT_EQ(str::is_literal_true("true"), true);
        ASSERT_EQ(str::is_literal_true("false"), false);
        ASSERT_EQ(str::is_literal_true("TRUE"), true);
        ASSERT_EQ(str::is_literal_true("FALSE"), false);
    }
    SECTION("空串") {
        ASSERT_EQ(str::is_literal_true(""), false);
    }
    SECTION("非标值") {
        ASSERT_EQ(str::is_literal_true("FalsE"), false);
        ASSERT_EQ(str::is_literal_true("tRUe"), false);
        ASSERT_EQ(str::is_literal_true("true "), false);
    }
    SECTION("超长值") {
        ASSERT_EQ(str::is_literal_true("hello-workd"), false);
    }
}
