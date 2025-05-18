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

TEST(test_str, is_lower) {
    SECTION("全小写") {
        ASSERT_EQ(str::is_lower("abcdef"), true);
    }
    SECTION("只有一个大写") {
        ASSERT_EQ(str::is_lower("abcdeF"), false);
    }
    SECTION("无字母") {
        ASSERT_EQ(str::is_lower("092320914#$@#$@35"), false);
    }
    SECTION("混合") {
        ASSERT_EQ(str::is_lower("abcDEF0923"), false);
    }
    SECTION("空串") {
        ASSERT_EQ(str::is_lower(""), false);
    }
}
