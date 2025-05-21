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
#include "test-compares.hpp"

#include <map>

TEST(test_str, split_pair) {
    SECTION("一般情况") {
        ASSERT_EQ(str::split_pair("a:b"), (std::tuple{"a", "b"}));
        ASSERT_EQ(str::split_pair("a:"), (std::tuple{"a", ""}));
        ASSERT_EQ(str::split_pair(":b"), (std::tuple{"", "b"}));
        ASSERT_EQ(str::split_pair(":"), (std::tuple{"", ""}));
        ASSERT_EQ(str::split_pair("ab"), (std::tuple{"ab", ""}));
        ASSERT_EQ(str::split_pair(""), (std::tuple{"", ""}));
    }
    SECTION("指定分隔符") {
        ASSERT_EQ(str::split_pair("a=b", "="), (std::tuple{"a", "b"}));
        ASSERT_EQ(str::split_pair("a=", "="), (std::tuple{"a", ""}));
        ASSERT_EQ(str::split_pair("=b", "="), (std::tuple{"", "b"}));
        ASSERT_EQ(str::split_pair("=", "="), (std::tuple{"", ""}));
        ASSERT_EQ(str::split_pair("ab", "="), (std::tuple{"ab", ""}));
        ASSERT_EQ(str::split_pair("", "="), (std::tuple{"", ""}));
    }
    SECTION("空分隔符") {
        ASSERT_EQ(str::split_pair("a:b", ""), (std::tuple{"a", "b"}));
    }
}