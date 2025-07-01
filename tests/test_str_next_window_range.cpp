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

TEST(test_str, next_window_range) {
    SECTION("一般情况") {
        size_t pos = 0;
        std::optional<str::range_type> result;

        result = str::next_window_range("abc-def-mnp-rst-uv", pos, 4, 3);
        ASSERT_TRUE(result && (*result == str::range(0, 4)) && (pos == 3));

        result = str::next_window_range("abc-def-mnp-rst-uv", pos, 4, 3);
        ASSERT_TRUE(result && (*result == str::range(3, 4)) && (pos == 6));

        result = str::next_window_range("abc-def-mnp-rst-uv", pos, 4, 3);
        ASSERT_TRUE(result && (*result == str::range(6, 4)) && (pos == 9));

        result = str::next_window_range("abc-def-mnp-rst-uv", pos, 4, 3);
        ASSERT_TRUE(result && (*result == str::range(9, 4)) && (pos == 12));

        result = str::next_window_range("abc-def-mnp-rst-uv", pos, 4, 3);
        ASSERT_TRUE(result && (*result == str::range(12, 4)) && (pos == 15));

        result = str::next_window_range("abc-def-mnp-rst-uv", pos, 4, 3);
        ASSERT_TRUE(result && (*result == str::range(15, 3)) && (pos == 18));

        result = str::next_window_range("abc-def-mnp-rst-uv", pos, 4, 3);
        ASSERT_TRUE(!result && (pos == 18));

        result = str::next_window_range("abc-def-mnp-rst-uv", pos = 18, 4, 3);
        ASSERT_TRUE(!result && (pos == 18));

        result = str::next_window_range("abc-def-mnp-rst-uv", pos = 20, 4, 3);
        ASSERT_TRUE(!result && (pos == 18));

        result = str::next_window_range("abc-def-mnp-rst-uv", pos = str::npos, 4, 3);
        ASSERT_TRUE(!result && (pos == 18));
    }
    SECTION("max_n") {
        size_t pos = 0;
        std::optional<str::range_type> result;

        // max_n = 0
        result = str::next_window_range("abc-def-mnp-rst-uv", pos = 0, 0, 3);
        ASSERT_TRUE(result && (*result == str::range(0, 18)) && (pos == 3));

        // max_n = 18
        result = str::next_window_range("abc-def-mnp-rst-uv", pos = 0, 18, 3);
        ASSERT_TRUE(result && (*result == str::range(0, 18)) && (pos == 3));

        // max_n = 20
        result = str::next_window_range("abc-def-mnp-rst-uv", pos = 0, 20, 3);
        ASSERT_TRUE(result && (*result == str::range(0, 18)) && (pos == 3));

        // max_n = str::npos
        result = str::next_window_range("abc-def-mnp-rst-uv", pos = 0, str::npos, 3);
        ASSERT_TRUE(result && (*result == str::range(0, 18)) && (pos == 3));
    }
    SECTION("空串") {
        size_t pos = 0;
        std::optional<str::range_type> result;

        result = str::next_window_range("", pos = 0, 0, 3);
        ASSERT_TRUE(!result && (pos == 0));

        result = str::next_window_range("", pos = 1, 0, 3);
        ASSERT_TRUE(!result && (pos == 0));

        result = str::next_window_range("", pos = str::npos, 0, 3);
        ASSERT_TRUE(!result && (pos == 0));
    }
}