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

TEST(test_str, skip_max) {
    SECTION("长度：剩余长度足够") {
        size_t pos = 0;
        std::optional<str::size_type> len;
        ASSERT_TRUE(len = str::skip_max("abcdefghi",pos = 0, 9));
        ASSERT_EQ(*len, 9);
        ASSERT_EQ(pos, 9);
        ASSERT_TRUE(len = str::skip_max("abcdefghi",pos = 3, 4));
        ASSERT_EQ(*len, 4);
        ASSERT_EQ(pos, 7);
        ASSERT_TRUE(len = str::skip_max("abcdefghi",pos = 3, 6));
        ASSERT_EQ(*len, 6);
        ASSERT_EQ(pos, 9);
        ASSERT_TRUE(len = str::skip_max("abcdefghi",pos = 3, 0));
        ASSERT_EQ(*len, 0);
        ASSERT_EQ(pos, 3);
    }
    SECTION("长度：剩余长度不够") {
        size_t pos = 0;
        std::optional<str::size_type> len;
        ASSERT_TRUE(len = str::skip_max("abcdefghi",pos = 3, 7));
        ASSERT_EQ(*len, 6);
        ASSERT_EQ(pos, 9);
        ASSERT_TRUE(len = str::skip_max("abcdefghi",pos = 3, 10));
        ASSERT_EQ(*len, 6);
        ASSERT_EQ(pos, 9);
        ASSERT_TRUE(len = str::skip_max("abcdefghi",pos = 3, str::npos));
        ASSERT_EQ(*len, 6);
        ASSERT_EQ(pos, 9);
    }
    SECTION("pos：超限") {
        size_t pos = 0;
        std::optional<str::size_type> len;
        ASSERT_TRUE(len = str::skip_max("abcdefghi",pos = 9, 0));
        ASSERT_EQ(*len, 0);
        ASSERT_EQ(pos, 9);
        ASSERT_TRUE(len = str::skip_max("abcdefghi",pos = 9, 1));
        ASSERT_EQ(*len, 0);
        ASSERT_EQ(pos, 9);
        ASSERT_FALSE(len = str::skip_max("abcdefghi",pos = 10, 0));
        ASSERT_EQ(pos, 10);
        ASSERT_FALSE(len = str::skip_max("abcdefghi",pos = 10, 1));
        ASSERT_EQ(pos, 10);
        ASSERT_FALSE(len = str::skip_max("abcdefghi",pos = str::npos, 1));
        ASSERT_EQ(pos, str::npos);
    }
    SECTION("空串") {
        size_t pos = 0;
        std::optional<str::size_type> len;
        ASSERT_TRUE(len = str::skip_max("",pos = 0, 0));
        ASSERT_EQ(*len, 0);
        ASSERT_EQ(pos, 0);
        ASSERT_TRUE(len = str::skip_max("",pos = 0, 1));
        ASSERT_EQ(*len, 0);
        ASSERT_EQ(pos, 0);
        ASSERT_FALSE(len = str::skip_max("",pos = 1, 0));
        ASSERT_EQ(pos, 1);
        ASSERT_FALSE(len = str::skip_max("",pos = str::npos, 0));
        ASSERT_EQ(pos, str::npos);
    }
}
