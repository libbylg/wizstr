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

TEST(test_str, prev_string_range) {
    SECTION("不同起始位置") {
        str::size_type pos = 0;
        std::optional<str::range_type> result;

        ASSERT_FALSE(result = str::prev_string_range("abc def", pos = 0, "de"));
        ASSERT_TRUE((pos == 0));

        ASSERT_FALSE(result = str::prev_string_range("abc def", pos = 1, "de"));
        ASSERT_TRUE((pos == 0));

        ASSERT_FALSE(result = str::prev_string_range("abc def", pos = 2, "de"));
        ASSERT_TRUE((pos == 0));

        ASSERT_FALSE(result = str::prev_string_range("abc def", pos = 3, "de"));
        ASSERT_TRUE((pos == 0));

        ASSERT_FALSE(result = str::prev_string_range("abc def", pos = 4, "de"));
        ASSERT_TRUE((pos == 0));

        ASSERT_TRUE(result = str::prev_string_range("abc def", pos = 5, "de"));
        ASSERT_TRUE((pos == 4) && (*result == str::range_type{4, 2}));

        ASSERT_TRUE(result = str::prev_string_range("abc def", pos = 6, "de"));
        ASSERT_TRUE((pos == 4) && (*result == str::range_type{4, 2}));

        ASSERT_TRUE(result = str::prev_string_range("abc def", pos = 7, "de"));
        ASSERT_TRUE((pos == 4) && (*result == str::range_type{4, 2}));

        ASSERT_TRUE(result = str::prev_string_range("abc def", pos = str::npos, "de"));
        ASSERT_TRUE((pos == 4) && (*result == str::range_type{4, 2}));
    }
    SECTION("无法匹配") {
        str::size_type pos = 0;
        std::optional<str::range_type> result;

        ASSERT_FALSE(result = str::prev_string_range("abc def", pos = str::npos, "KK"));
        ASSERT_TRUE((pos == 0));

        ASSERT_FALSE(result = str::prev_string_range("ab", pos = str::npos, "abc"));
        ASSERT_TRUE((pos == 0));

        ASSERT_FALSE(result = str::prev_string_range("ab", pos = str::npos, "bcd"));
        ASSERT_TRUE((pos == 0));
    }
    SECTION("在空串中查找") {
        str::size_type pos = 0;
        std::optional<str::range_type> result;

        ASSERT_FALSE(result = str::prev_string_range("", pos = 0, "a"));
        ASSERT_TRUE(pos == 0);

        ASSERT_FALSE(result = str::prev_string_range("", pos = str::npos, ""));
        ASSERT_TRUE(pos == 0);
    }
    SECTION("查找空串") {
        str::size_type pos = 0;
        std::optional<str::range_type> result;

        ASSERT_FALSE(result = str::prev_string_range("abc", pos = 0, ""));
        ASSERT_TRUE(pos == 0);

        ASSERT_FALSE(result = str::prev_string_range("abcdef", pos = str::npos, ""));
        ASSERT_TRUE(pos == 0);
    }
    SECTION("被查找字符串有自重叠") {
        str::size_type pos = str::npos;
        std::optional<str::range_type> result;

        ASSERT_TRUE(result = str::prev_string_range("AAAKAAAAA", pos, "AAA"));
        ASSERT_TRUE((pos == 6) && (*result == str::range_type{6, 3}));

        ASSERT_TRUE(result = str::prev_string_range("AAAKAAAAA", pos, "AAA"));
        ASSERT_TRUE((pos == 5) && (*result == str::range_type{5, 3}));

        ASSERT_TRUE(result = str::prev_string_range("AAAKAAAAA", pos, "AAA"));
        ASSERT_TRUE((pos == 4) && (*result == str::range_type{4, 3}));

        ASSERT_TRUE(result = str::prev_string_range("AAAKAAAAA", pos, "AAA"));
        ASSERT_TRUE((pos == 0) && (*result == str::range_type{0, 3}));

        ASSERT_TRUE(result = str::next_string_range("AAAKAAAAA", pos, "AAA"));
        ASSERT_TRUE((pos == 1) && (*result == str::range_type{0, 3}));

        ASSERT_TRUE(result = str::next_string_range("AAAKAAAAA", pos, "AAA"));
        ASSERT_TRUE((pos == 5) && (*result == str::range_type{4, 3}));

        ASSERT_TRUE(result = str::next_string_range("AAAKAAAAA", pos, "AAA"));
        ASSERT_TRUE((pos == 6) && (*result == str::range_type{5, 3}));

        ASSERT_TRUE(result = str::next_string_range("AAAKAAAAA", pos, "AAA"));
        ASSERT_TRUE((pos == 7) && (*result == str::range_type{6, 3}));

        ASSERT_FALSE(result = str::next_string_range("AAAKAAAAA", pos, "AAA"));
        ASSERT_TRUE((pos == 9));
    }
}
