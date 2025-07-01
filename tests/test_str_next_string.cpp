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

#if defined(STR_NAMESPACE)
using str = STR_NAMESPACE::str;
#endif

TEST(test_str, next_string) {
    SECTION("一般场景") {
        std::string_view s{"AAabcAAdefAAAA"};
        str::size_type pos = 0;
        std::optional<std::string> result;

        ASSERT_TRUE(result = str::next_string(s, pos, "AA"));
        ASSERT_TRUE((pos == 2) && (*result == "AA"));

        ASSERT_TRUE(result = str::next_string(s, pos, "AA"));
        ASSERT_TRUE((pos == 7) && (*result == "AA"));

        ASSERT_TRUE(result = str::next_string(s, pos, "AA"));
        ASSERT_TRUE((pos == 12) && (*result == "AA"));

        ASSERT_TRUE(result = str::next_string(s, pos, "AA"));
        ASSERT_TRUE((pos == 14) && (*result == "AA"));

        ASSERT_FALSE(result = str::next_string(s, pos, "AA"));
        ASSERT_TRUE((pos == 14));

        ASSERT_FALSE(result = str::next_string(s, pos, "AA"));
        ASSERT_TRUE((pos == 14));
    }
    SECTION("pos超过范围:1") {
        std::string_view s{"AAabcAAdefAAAA"};
        size_t pos = 14;
        std::optional<std::string> result;

        ASSERT_FALSE(result = str::next_string(s, pos, "AA"));
        ASSERT_TRUE((pos == 14));
    }
    SECTION("pos超过范围:npos") {
        std::string_view s{"AAabcAAdefAAAA"};
        size_t pos = str::npos;
        std::optional<std::string> result;

        ASSERT_FALSE(result = str::next_string(s, pos, "AA"));
        ASSERT_TRUE((pos == 14));
    }
    SECTION("在空串中查找") {
        std::string_view s{""};
        size_t pos = 14;
        std::optional<std::string> result;

        ASSERT_FALSE(result = str::next_string(s, pos, "AA"));
        ASSERT_TRUE((pos == 0));
    }
    SECTION("查找空串") {
        std::string_view s{"abc"};
        str::size_type pos = 0;
        std::optional<std::string> result;

        ASSERT_FALSE(result = str::next_string(s, pos, ""));
        ASSERT_TRUE((pos == 3));

        ASSERT_FALSE(result = str::next_string(s, pos, ""));
        ASSERT_TRUE((pos == 3));
    }
}