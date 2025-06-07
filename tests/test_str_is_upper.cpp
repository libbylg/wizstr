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

TEST(test_str, is_upper) {
    SECTION("全大写") {
        ASSERT_EQ(str::is_upper("ABCDEF"), true);
    }
    SECTION("只有一个小写") {
        ASSERT_EQ(str::is_upper("ABCDEf"), false);
    }
    SECTION("无字母") {
        ASSERT_EQ(str::is_upper("092320914#$@#$@35"), false);
    }
    SECTION("混合") {
        ASSERT_EQ(str::is_upper("abcDEF0923"), false);
    }
    SECTION("空串") {
        ASSERT_EQ(str::is_upper(""), false);
    }
}