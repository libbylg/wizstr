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

TEST(test_str, prev_string) {
    SECTION("不同起始位置") {
        str::size_type pos = 0;
        std::optional<std::string> result;

        ASSERT_FALSE(result = str::prev_string("abc def", pos = 0, "de"));
        ASSERT_TRUE((pos == 0));

        ASSERT_FALSE(result = str::prev_string("abc def", pos = 1, "de"));
        ASSERT_TRUE((pos == 0));

        ASSERT_FALSE(result = str::prev_string("abc def", pos = 2, "de"));
        ASSERT_TRUE((pos == 0));

        ASSERT_FALSE(result = str::prev_string("abc def", pos = 3, "de"));
        ASSERT_TRUE((pos == 0));

        ASSERT_FALSE(result = str::prev_string("abc def", pos = 4, "de"));
        ASSERT_TRUE((pos == 0));

        ASSERT_TRUE(result = str::prev_string("abc def", pos = 5, "de"));
        ASSERT_TRUE((pos == 4) && (*result == "de"));

        ASSERT_TRUE(result = str::prev_string("abc def", pos = 6, "de"));
        ASSERT_TRUE((pos == 4) && (*result == "de"));

        ASSERT_TRUE(result = str::prev_string("abc def", pos = 7, "de"));
        ASSERT_TRUE((pos == 4) && (*result == "de"));

        ASSERT_TRUE(result = str::prev_string("abc def", pos = str::npos, "de"));
        ASSERT_TRUE((pos == 4) && (*result == "de"));
    }
    SECTION("无法匹配") {
        str::size_type pos = 0;
        std::optional<std::string> result;

        ASSERT_FALSE(result = str::prev_string("abc def", pos = str::npos, "KK"));
        ASSERT_TRUE((pos == 0));

        ASSERT_FALSE(result = str::prev_string("ab", pos = str::npos, "abc"));
        ASSERT_TRUE((pos == 0));

        ASSERT_FALSE(result = str::prev_string("ab", pos = str::npos, "bcd"));
        ASSERT_TRUE((pos == 0));
    }
    SECTION("在空串中查找") {
        str::size_type pos = 0;
        std::optional<std::string> result;

        ASSERT_FALSE(result = str::prev_string("", pos = 0, "a"));
        ASSERT_TRUE(pos == 0);

        ASSERT_FALSE(result = str::prev_string("", pos = str::npos, ""));
        ASSERT_TRUE(pos == 0);
    }
    SECTION("查找空串") {
        str::size_type pos = 0;
        std::optional<std::string> result;

        ASSERT_FALSE(result = str::prev_string("abc", pos = 0, ""));
        ASSERT_TRUE(pos == 0);

        ASSERT_FALSE(result = str::prev_string("abcdef", pos = str::npos, ""));
        ASSERT_TRUE(pos == 0);
    }
}
