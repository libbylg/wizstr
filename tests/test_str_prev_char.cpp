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

TEST(test_str, prev_char) {
    SECTION("查找前一个字符") {
        str::size_type pos = 0;
        std::optional<str::size_type> result;

        ASSERT_FALSE(result = str::prev_char("abc def", pos = 0, 'd'));
        ASSERT_TRUE((pos == 0));
        ASSERT_FALSE(result = str::prev_char("abc def", pos = 1, 'd'));
        ASSERT_TRUE((pos == 0));
        ASSERT_FALSE(result = str::prev_char("abc def", pos = 2, 'd'));
        ASSERT_TRUE((pos == 0));
        ASSERT_FALSE(result = str::prev_char("abc def", pos = 3, 'd'));
        ASSERT_TRUE((pos == 0));
        ASSERT_FALSE(result = str::prev_char("abc def", pos = 4, 'd'));
        ASSERT_TRUE((pos == 0) && (!result));
        ASSERT_TRUE(result = str::prev_char("abc def", pos = 5, 'd'));
        ASSERT_TRUE((pos == 4) && (*result == 4));
        ASSERT_TRUE(result = str::prev_char("abc def", pos = 6, 'd'));
        ASSERT_TRUE((pos == 4) && (*result == 4));
        ASSERT_TRUE(result = str::prev_char("abc def", pos = 7, 'd'));
        ASSERT_TRUE((pos == 4) && (*result == 4));
        ASSERT_TRUE(result = str::prev_char("abc def", pos = str::npos, 'd'));
        ASSERT_TRUE((pos == 4) && (*result == 4));
        ASSERT_FALSE(result = str::prev_char("abc def", pos = 0, 'K'));
        ASSERT_TRUE((pos == 0));
        ASSERT_FALSE(result = str::prev_char("", pos = 0, '\0'));
        ASSERT_TRUE((pos == 0) && (!result));
        ASSERT_FALSE(result = str::prev_char("", pos = str::npos, '\0'));
        ASSERT_TRUE((pos == 0) && (!result));
    }
    SECTION("查找前一个字符集") {
        str::size_type pos = 0;
        std::optional<str::size_type> result;
        ASSERT_FALSE(result = str::prev_char("abc def", pos = 0, "#db"));
        ASSERT_TRUE((pos == 0));
        ASSERT_FALSE(result = str::prev_char("abc def", pos = 1, "#db"));
        ASSERT_TRUE((pos == 0));
        ASSERT_TRUE(result = str::prev_char("abc def", pos = 2, "#db"));
        ASSERT_TRUE((pos == 1) && (*result == 1));
        ASSERT_TRUE(result = str::prev_char("abc def", pos = 3, "#db"));
        ASSERT_TRUE((pos == 1) && (*result == 1));
        ASSERT_TRUE(result = str::prev_char("abc def", pos = 4, "#db"));
        ASSERT_TRUE((pos == 1) && (*result == 1));
        ASSERT_TRUE(result = str::prev_char("abc def", pos = 5, "#db"));
        ASSERT_TRUE((pos == 4) && (*result == 4));
        ASSERT_TRUE(result = str::prev_char("abc def", pos = 6, "#db"));
        ASSERT_TRUE((pos == 4) && (*result == 4));
        ASSERT_TRUE(result = str::prev_char("abc def", pos = 7, "#db"));
        ASSERT_TRUE((pos == 4) && (*result == 4));
        ASSERT_TRUE(result = str::prev_char("abc def", pos = str::npos, "#bd"));
        ASSERT_TRUE((pos == 4) && (*result == 4));
        ASSERT_FALSE(result = str::prev_char("abc def", pos = 0, "#K"));
        ASSERT_TRUE((pos == 0));
        ASSERT_FALSE(result = str::prev_char("", pos = 0, std::string_view{"\0", 1}));
        ASSERT_TRUE((pos == 0));
        ASSERT_FALSE(result = str::prev_char("", pos = str::npos, std::string_view{"\0", 1}));
        ASSERT_TRUE((pos == 0));
    }
    SECTION("查找满足前一个proc的字符的位置") {
        str::size_type pos = 0;
        std::optional<str::size_type> result;
        ASSERT_FALSE(result = str::prev_char("a12 e3", pos = 0, [](str::value_type ch) -> bool {
            return std::isdigit(ch);
        }));
        ASSERT_EQ(pos, 0);
        ASSERT_FALSE(result = str::prev_char("a12 e3", pos = 1, [](str::value_type ch) -> bool {
            return std::isdigit(ch);
        }));
        ASSERT_EQ(pos, 0);
        ASSERT_TRUE(result = str::prev_char("a12 e3", pos = 2, [](str::value_type ch) -> bool {
            return std::isdigit(ch);
        }));
        ASSERT_TRUE((pos == 1) && (*result == 1));
        ASSERT_TRUE(result = str::prev_char("a12 e3", pos = 3, [](str::value_type ch) -> bool {
            return std::isdigit(ch);
        }));
        ASSERT_TRUE((pos == 2) && (*result == 2));
        ASSERT_TRUE(result = str::prev_char("a12 e3", pos = 4, [](str::value_type ch) -> bool {
            return std::isdigit(ch);
        }));
        ASSERT_TRUE((pos == 2) && (*result == 2));
        ASSERT_TRUE(result = str::prev_char("a12 e3", pos = 5, [](str::value_type ch) -> bool {
            return std::isdigit(ch);
        }));
        ASSERT_TRUE((pos == 2) && (*result == 2));
        ASSERT_TRUE(result = str::prev_char("a12 e3", pos = 6, [](str::value_type ch) -> bool {
            return std::isdigit(ch);
        }));
        ASSERT_TRUE((pos == 5) && (*result == 5));
        ASSERT_TRUE(result = str::prev_char("a12 e3", pos = 7, [](str::value_type ch) -> bool {
            return std::isdigit(ch);
        }));
        ASSERT_TRUE((pos == 5) && (*result == 5));
        ASSERT_TRUE(result = str::prev_char("a12 e3", pos = str::npos, [](str::value_type ch) -> bool {
            return std::isdigit(ch);
        }));
        ASSERT_TRUE((pos == 5) && (*result == 5));
        ASSERT_FALSE(result = str::prev_char("a12 e3", pos = str::npos, []([[maybe_unused]] str::value_type ch) -> bool {
            return false;
        }));
        ASSERT_EQ(pos, 0);
    }
}
