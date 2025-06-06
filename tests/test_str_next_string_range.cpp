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

TEST(test_str, next_string_range) {
    SECTION("不同起始位置") {
        str::size_type pos = 0;
        std::optional<str::range_type> result;

        ASSERT_TRUE(result = str::next_string_range("Aabc ab Kaba", pos = 0, "ab"));
        ASSERT_TRUE((pos == 2) && (*result == str::range_type{1, 2}));
        ASSERT_TRUE(result = str::next_string_range("Aabc ab Kaba", pos = 1, "ab"));
        ASSERT_TRUE((pos == 2) && (*result == str::range_type{1, 2}));
        ASSERT_TRUE(result = str::next_string_range("Aabc ab Kaba", pos = 2, "ab"));
        ASSERT_TRUE((pos == 6) && (*result == str::range_type{5, 2}));
        ASSERT_TRUE(result = str::next_string_range("Aabc ab Kaba", pos = 3, "ab"));
        ASSERT_TRUE((pos == 6) && (*result == str::range_type{5, 2}));
        ASSERT_TRUE(result = str::next_string_range("Aabc ab Kaba", pos = 4, "ab"));
        ASSERT_TRUE((pos == 6) && (*result == str::range_type{5, 2}));
        ASSERT_TRUE(result = str::next_string_range("Aabc ab Kaba", pos = 5, "ab"));
        ASSERT_TRUE((pos == 6) && (*result == str::range_type{5, 2}));
        ASSERT_TRUE(result = str::next_string_range("Aabc ab Kaba", pos = 6, "ab"));
        ASSERT_TRUE((pos == 10) && (*result == str::range_type{9, 2}));
        ASSERT_TRUE(result = str::next_string_range("Aabc ab Kaba", pos = 7, "ab"));
        ASSERT_TRUE((pos == 10) && (*result == str::range_type{9, 2}));
        ASSERT_FALSE(result = str::next_string_range("Aabc ab Kaba", pos = 10, "ab"));
        ASSERT_TRUE((pos == 12));
        ASSERT_FALSE(result = str::next_string_range("Aabc ab Kaba", pos = str::npos, "ab"));
        ASSERT_TRUE((pos == 12));
    }
    SECTION("在空串中查找") {
        str::size_type pos = 0;
        std::optional<str::range_type> result;

        ASSERT_FALSE(result = str::next_string_range("", pos = 0, "ab"));
        ASSERT_TRUE(pos == 0);
        ASSERT_FALSE(result = str::next_string_range("", pos = str::npos, "ab"));
        ASSERT_TRUE(pos == 0);
    }
    SECTION("查找空串") {
        str::size_type pos = 0;
        std::optional<str::range_type> result;

        ASSERT_FALSE(result = str::next_string_range("ab", pos = 0, ""));
        ASSERT_TRUE(pos == 2);
        ASSERT_FALSE(result = str::next_string_range("ab", pos = 1, ""));
        ASSERT_TRUE(pos == 2);
        ASSERT_FALSE(result = str::next_string_range("ab", pos = 2, ""));
        ASSERT_TRUE(pos == 2);
        ASSERT_FALSE(result = str::next_string_range("ab", pos = 3, ""));
        ASSERT_TRUE(pos == 2);
        ASSERT_FALSE(result = str::next_string_range("abc", pos = str::npos, ""));
        ASSERT_TRUE(pos == 3);
        ASSERT_FALSE(result = str::next_string_range("", pos = str::npos, ""));
        ASSERT_TRUE(pos == 0);
        ASSERT_FALSE(result = str::next_string_range("", pos = 0, ""));
        ASSERT_TRUE(pos == 0);
    }
    SECTION("被查找的目标串有重叠") {
        str::size_type pos = 0;
        std::optional<str::range_type> result;
        ASSERT_TRUE(result = str::next_string_range("abababab", pos, "aba"));
        ASSERT_TRUE(pos == 1);
        ASSERT_TRUE(result = str::next_string_range("abababab", pos, "aba"));
        ASSERT_TRUE(pos == 3);
        ASSERT_TRUE(result = str::next_string_range("abababab", pos, "aba"));
        ASSERT_TRUE(pos == 5);
        ASSERT_FALSE(result = str::next_string_range("abababab", pos, "aba"));
        ASSERT_TRUE(pos == 8);
        ASSERT_FALSE(result = str::next_string_range("abababab", pos, "aba"));
        ASSERT_TRUE(pos == 8);
    }
    SECTION("prev_xx与next_xx配合操作") {
        str::size_type pos = 0;
        std::optional<str::range_type> result;
        std::string s{"abababa"};
        ASSERT_TRUE(result = str::next_string_range(s, pos, "aba"));
        ASSERT_TRUE((pos == 1) && (*result == str::range_type{0, 3}));

        ASSERT_TRUE(result = str::next_string_range(s, pos, "aba"));
        ASSERT_TRUE((pos == 3) && (*result == str::range_type{2, 3}));

        ASSERT_TRUE(result = str::next_string_range(s, pos, "aba"));
        ASSERT_TRUE((pos == 5) && (*result == str::range_type{4, 3}));

        ASSERT_FALSE(result = str::next_string_range(s, pos, "aba"));
        ASSERT_TRUE((pos == 7));

        ASSERT_TRUE(result = str::prev_string_range(s, pos, "aba"));
        ASSERT_TRUE((pos == 4) && (*result == str::range_type{4, 3}));

        ASSERT_TRUE(result = str::prev_string_range(s, pos, "aba"));
        ASSERT_TRUE((pos == 2) && (*result == str::range_type{2, 3}));

        ASSERT_TRUE(result = str::prev_string_range(s, pos, "aba"));
        ASSERT_TRUE((pos == 0));

        ASSERT_FALSE(result = str::prev_string_range(s, pos, "aba"));
        ASSERT_TRUE((pos == 0));
    }
}
