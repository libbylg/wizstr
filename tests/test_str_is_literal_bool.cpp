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

TEST(test_str, is_literal_bool) {
    SECTION("标准值") {
        ASSERT_TRUE(str::is_literal_bool("1"));
        ASSERT_TRUE(str::is_literal_bool("0"));
        ASSERT_TRUE(str::is_literal_bool("on"));
        ASSERT_TRUE(str::is_literal_bool("off"));
        ASSERT_TRUE(str::is_literal_bool("ON"));
        ASSERT_TRUE(str::is_literal_bool("OFF"));
        ASSERT_TRUE(str::is_literal_bool("On"));
        ASSERT_TRUE(str::is_literal_bool("Off"));
        ASSERT_TRUE(str::is_literal_bool("Yes"));
        ASSERT_TRUE(str::is_literal_bool("No"));
        ASSERT_TRUE(str::is_literal_bool("yes"));
        ASSERT_TRUE(str::is_literal_bool("no"));
        ASSERT_TRUE(str::is_literal_bool("YES"));
        ASSERT_TRUE(str::is_literal_bool("NO"));
        ASSERT_TRUE(str::is_literal_bool("True"));
        ASSERT_TRUE(str::is_literal_bool("False"));
        ASSERT_TRUE(str::is_literal_bool("true"));
        ASSERT_TRUE(str::is_literal_bool("false"));
        ASSERT_TRUE(str::is_literal_bool("TRUE"));
        ASSERT_TRUE(str::is_literal_bool("FALSE"));
    }
    SECTION("空串") {
        ASSERT_EQ(str::is_literal_bool(""), false);
    }
    SECTION("非标值") {
        ASSERT_EQ(str::is_literal_bool("FalsE"), false);
        ASSERT_EQ(str::is_literal_bool("tRUe"), false);
        ASSERT_EQ(str::is_literal_bool("true "), false);
    }
    SECTION("超长值") {
        ASSERT_EQ(str::is_literal_bool("hello-workd"), false);
    }
}
