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

TEST(test_str, is_blank) {
    ASSERT_EQ(str::is_blank(" \t\t "), true);
    ASSERT_EQ(str::is_blank("\r\n\v\t "), false);
    ASSERT_EQ(str::is_blank(""), false);
    ASSERT_EQ(str::is_blank("XYZ#"), false);
    ASSERT_EQ(str::is_blank("\t  XYZ#"), false);
    ASSERT_EQ(str::is_blank("XYZ#\t  "), false);
}
