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

TEST(test_str, skip_spaces) {
    size_t pos = 0;

    str::skip_spaces("abc", pos = 0);
    ASSERT_EQ(pos, 0);
    str::skip_spaces(" abc", pos = 0);
    ASSERT_EQ(pos, 1);
    str::skip_spaces(" \r\n\tabc", pos = 0);
    ASSERT_EQ(pos, 4);
    str::skip_spaces(" \r\n\t", pos = 0);
    ASSERT_EQ(pos, 4);
    str::skip_spaces(" \r\n\t", pos = 9);
    ASSERT_EQ(pos, 4);
    str::skip_spaces("", pos = 0);
    ASSERT_EQ(pos, 0);
}
