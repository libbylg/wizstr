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

TEST(test_str, next_chunk_range) {
    SECTION("一般情况") {
        size_t pos = 0;
        std::optional<str::range_type> result;

        result = str::next_chunk_range("abc-def-mnp-rst-uv", pos, 4);
        ASSERT_TRUE(result && (*result == str::range(0, 4)) && (pos == 4));

        result = str::next_chunk_range("abc-def-mnp-rst-uv", pos, 4);
        ASSERT_TRUE(result && (*result == str::range(4, 4)) && (pos == 8));

        result = str::next_chunk_range("abc-def-mnp-rst-uv", pos, 4);
        ASSERT_TRUE(result && (*result == str::range(8, 4)) && (pos == 12));

        result = str::next_chunk_range("abc-def-mnp-rst-uv", pos, 4);
        ASSERT_TRUE(result && (*result == str::range(12, 4)) && (pos == 16));

        result = str::next_chunk_range("abc-def-mnp-rst-uv", pos, 4);
        ASSERT_TRUE(result && (*result == str::range(16, 2)) && (pos == 18));

        result = str::next_chunk_range("abc-def-mnp-rst-uv", pos, 4);
        ASSERT_TRUE(!result && (pos == 18));

        result = str::next_chunk_range("abc-def-mnp-rst-uv", pos = 18, 4);
        ASSERT_TRUE(!result && (pos == 18));

        result = str::next_chunk_range("abc-def-mnp-rst-uv", pos = 20, 4);
        ASSERT_TRUE(!result && (pos == 18));

        result = str::next_chunk_range("abc-def-mnp-rst-uv", pos = str::npos, 4);
        ASSERT_TRUE(!result && (pos == 18));
    }
    SECTION("max_n") {
        size_t pos = 0;
        std::optional<str::range_type> result;

        // max_n = 0
        result = str::next_chunk_range("abc-def-mnp-rst-uv", pos = 0, 0);
        ASSERT_TRUE(result && (*result == str::range(0, 18)) && (pos == 18));

        // max_n = 18
        result = str::next_chunk_range("abc-def-mnp-rst-uv", pos = 0, 18);
        ASSERT_TRUE(result && (*result == str::range(0, 18)) && (pos == 18));

        // max_n = 20
        result = str::next_chunk_range("abc-def-mnp-rst-uv", pos = 0, 18);
        ASSERT_TRUE(result && (*result == str::range(0, 18)) && (pos == 18));

        // max_n = str::npos
        result = str::next_chunk_range("abc-def-mnp-rst-uv", pos = 0, str::npos);
        ASSERT_TRUE(result && (*result == str::range(0, 18)) && (pos == 18));
    }
    SECTION("空串") {
        size_t pos = 0;
        std::optional<str::range_type> result;

        result = str::next_chunk_range("", pos = 0, 0);
        ASSERT_TRUE(!result && (pos == 0));

        result = str::next_chunk_range("", pos = 1, 0);
        ASSERT_TRUE(!result && (pos == 0));

        result = str::next_chunk_range("", pos = str::npos, 0);
        ASSERT_TRUE(!result && (pos == 0));
    }
}