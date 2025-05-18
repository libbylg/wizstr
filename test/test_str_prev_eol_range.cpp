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

TEST(test_str, prev_eol_range) {
    SECTION("一般情况:LF") {
        std::string_view s{"a\nb\nc"};
        str::size_type pos = str::npos;
        std::optional<str::range_type> result;

        ASSERT_TRUE(result = str::prev_eol_range(s, pos));
        ASSERT_TRUE((pos == 3) && (*result == str::range_type{3, 1}));
        ASSERT_TRUE(result = str::prev_eol_range(s, pos));
        ASSERT_TRUE((pos == 1) && (*result == str::range_type{1, 1}));
        ASSERT_FALSE(result = str::prev_eol_range(s, pos));
        ASSERT_TRUE((pos == 0));
        ASSERT_FALSE(result = str::prev_eol_range(s, pos));
        ASSERT_TRUE((pos == 0));
    }
    SECTION("\\r+\\n") {
        std::string_view s{"a\r\nb\nc"};
        str::size_type pos = str::npos;
        std::optional<str::range_type> result;

        ASSERT_TRUE(result = str::prev_eol_range(s, pos));
        ASSERT_TRUE((pos == 4) && (*result == str::range_type{4, 1}));
        ASSERT_TRUE(result = str::prev_eol_range(s, pos));
        ASSERT_TRUE((pos == 1) && (*result == str::range_type{1, 2}));
        ASSERT_FALSE(result = str::prev_eol_range(s, pos));
        ASSERT_TRUE((pos == 0));
        ASSERT_FALSE(result = str::prev_eol_range(s, pos));
        ASSERT_TRUE((pos == 0));
    }
    SECTION("\\r+\\r") {
        std::string_view s{"a\r\rb\nc"};
        str::size_type pos = str::npos;
        std::optional<str::range_type> result;

        ASSERT_TRUE(result = str::prev_eol_range(s, pos));
        ASSERT_TRUE((pos == 4) && (*result == str::range_type{4, 1}));
        ASSERT_TRUE(result = str::prev_eol_range(s, pos));
        ASSERT_TRUE((pos == 2) && (*result == str::range_type{2, 1}));
        ASSERT_TRUE(result = str::prev_eol_range(s, pos));
        ASSERT_TRUE((pos == 1) && (*result == str::range_type{1, 1}));
        ASSERT_FALSE(result = str::prev_eol_range(s, pos));
        ASSERT_TRUE((pos == 0));
        ASSERT_FALSE(result = str::prev_eol_range(s, pos));
        ASSERT_TRUE((pos == 0));
    }
    SECTION("\\n+\\n") {
        std::string_view s{"a\n\nb\nc"};
        str::size_type pos = str::npos;
        std::optional<str::range_type> result;

        ASSERT_TRUE(result = str::prev_eol_range(s, pos));
        ASSERT_TRUE((pos == 4) && (*result == str::range_type{4, 1}));
        ASSERT_TRUE(result = str::prev_eol_range(s, pos));
        ASSERT_TRUE((pos == 2) && (*result == str::range_type{2, 1}));
        ASSERT_TRUE(result = str::prev_eol_range(s, pos));
        ASSERT_TRUE((pos == 1) && (*result == str::range_type{1, 1}));
        ASSERT_FALSE(result = str::prev_eol_range(s, pos));
        ASSERT_TRUE((pos == 0));
        ASSERT_FALSE(result = str::prev_eol_range(s, pos));
        ASSERT_TRUE((pos == 0));
    }
    SECTION("\\n+\\r") {
        std::string_view s{"a\n\rb\nc"};
        str::size_type pos = str::npos;
        std::optional<str::range_type> result;

        ASSERT_TRUE(result = str::prev_eol_range(s, pos));
        ASSERT_TRUE((pos == 4) && (*result == str::range_type{4, 1}));
        ASSERT_TRUE(result = str::prev_eol_range(s, pos));
        ASSERT_TRUE((pos == 2) && (*result == str::range_type{2, 1}));
        ASSERT_TRUE(result = str::prev_eol_range(s, pos));
        ASSERT_TRUE((pos == 1) && (*result == str::range_type{1, 1}));
        ASSERT_FALSE(result = str::prev_eol_range(s, pos));
        ASSERT_TRUE((pos == 0));
        ASSERT_FALSE(result = str::prev_eol_range(s, pos));
        ASSERT_TRUE((pos == 0));
    }
    SECTION("空串") {
        std::string_view s{""};
        str::size_type pos = str::npos;
        std::optional<str::range_type> result;

        ASSERT_FALSE(result = str::prev_eol_range(s, pos));
        ASSERT_TRUE((pos == 0));
        ASSERT_FALSE(result = str::prev_eol_range(s, pos));
        ASSERT_TRUE((pos == 0));
    }
    SECTION("无任何换行符号") {
        std::string_view s{"abcdef"};
        str::size_type pos = str::npos;
        std::optional<str::range_type> result;

        ASSERT_FALSE(result = str::prev_eol_range(s, pos));
        ASSERT_TRUE((pos == 0));
        ASSERT_FALSE(result = str::prev_eol_range(s, pos));
        ASSERT_TRUE((pos == 0));
    }
    SECTION("单一\\r") {
        std::string_view s{"\r"};
        str::size_type pos = str::npos;
        std::optional<str::range_type> result;

        ASSERT_TRUE(result = str::prev_eol_range(s, pos));
        ASSERT_TRUE((pos == 0) && (*result == str::range_type{0, 1}));
        ASSERT_FALSE(result = str::prev_eol_range(s, pos));
        ASSERT_TRUE((pos == 0));
    }
    SECTION("单一\\n") {
        std::string_view s{"\n"};
        str::size_type pos = str::npos;
        std::optional<str::range_type> result;

        ASSERT_TRUE(result = str::prev_eol_range(s, pos));
        ASSERT_TRUE((pos == 0) && (*result == str::range_type{0, 1}));
        ASSERT_FALSE(result = str::prev_eol_range(s, pos));
        ASSERT_TRUE((pos == 0));
    }
    SECTION("综合") {
        std::string_view s{"\n\r\n\r\r\r\n"};
        str::size_type pos = str::npos;
        std::optional<str::range_type> result;

        ASSERT_TRUE(result = str::prev_eol_range(s, pos));
        ASSERT_TRUE((pos == 5) && (*result == str::range_type{5, 2}));
        ASSERT_TRUE(result = str::prev_eol_range(s, pos));
        ASSERT_TRUE((pos == 4) && (*result == str::range_type{4, 1}));
        ASSERT_TRUE(result = str::prev_eol_range(s, pos));
        ASSERT_TRUE((pos == 3) && (*result == str::range_type{3, 1}));
        ASSERT_TRUE(result = str::prev_eol_range(s, pos));
        ASSERT_TRUE((pos == 1) && (*result == str::range_type{1, 2}));
        ASSERT_TRUE(result = str::prev_eol_range(s, pos));
        ASSERT_TRUE((pos == 0) && (*result == str::range_type{0, 1}));
        ASSERT_FALSE(result = str::prev_eol_range(s, pos));
        ASSERT_TRUE((pos == 0));
        ASSERT_FALSE(result = str::prev_eol_range(s, pos));
        ASSERT_TRUE((pos == 0));
    }
    SECTION("pos超出字符串范围[1]") {
        std::string_view s{"aKb\nc"};
        str::size_type pos = 5;
        std::optional<str::range_type> result;

        ASSERT_TRUE(result = str::prev_eol_range(s, pos));
        ASSERT_TRUE((pos == 3) && (*result == str::range_type{3, 1}));
        ASSERT_FALSE(result = str::prev_eol_range(s, pos = 1));
        ASSERT_TRUE((pos == 0));
    }
    SECTION("pos超出字符串范围[2]") {
        std::string_view s{"aaKbb\nc"};
        str::size_type pos = 10;
        std::optional<str::range_type> result;

        ASSERT_TRUE(result = str::prev_eol_range(s, pos));
        ASSERT_TRUE((pos == 5) && (*result == str::range_type{5, 1}));
        ASSERT_FALSE(result = str::prev_eol_range(s, pos));
        ASSERT_TRUE((pos == 0));
    }
}