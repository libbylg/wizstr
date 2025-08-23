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

TEST(test_str, skip_space_view) {
    size_t pos{0};
    ASSERT_EQ(str::after_skip_spaces_view("abc"), "abc");
    ASSERT_EQ(str::after_skip_spaces_view("   abc  "), "abc  ");
    ASSERT_EQ(str::after_skip_spaces_view("   abc  ", pos = 1), "abc  ");
    ASSERT_EQ(str::after_skip_spaces_view("   abc  ", pos = 2), "abc  ");
    ASSERT_EQ(str::after_skip_spaces_view("   abc  ", pos = 3), "abc  ");
    ASSERT_EQ(str::after_skip_spaces_view("   abc  ", pos = 10), "");
    ASSERT_EQ(str::after_skip_spaces_view(" \r\t\n ", pos = 0), "");
    ASSERT_EQ(str::after_skip_spaces_view(""), "");
    ASSERT_EQ(str::after_skip_spaces_view("abc  def"), "abc  def");
}

#if defined(WIZSTR_NAMESPACE)
using str = WIZSTR_NAMESPACE::str;
#endif

TEST(test_str, skip_space) {
    size_t pos{0};
    ASSERT_EQ(str::after_skip_spaces("abc", pos = 0), "abc");
    // ASSERT_EQ(pos, 0);
    ASSERT_EQ(str::after_skip_spaces("   abc  ", pos = 0), "abc  ");
    // ASSERT_EQ(pos, 3);
    ASSERT_EQ(str::after_skip_spaces(" \r\t\n ", pos = 0), "");
    // ASSERT_EQ(pos, 5);
    ASSERT_EQ(str::after_skip_spaces("", pos = 0), "");
    // ASSERT_EQ(pos, 0);
    ASSERT_EQ(str::after_skip_spaces("abc  def", pos = 7), "f");
    // ASSERT_EQ(pos, 7);
    ASSERT_EQ(str::after_skip_spaces("abc  def", pos = 8), "");
    // ASSERT_EQ(pos, 8);
    ASSERT_EQ(str::after_skip_spaces("abc  def", pos = str::npos), "");
    // ASSERT_EQ(pos, 8);
}
