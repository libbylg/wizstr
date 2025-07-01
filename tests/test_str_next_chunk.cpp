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

TEST(test_str, next_chunk) {
    SECTION("一般情况") {
        size_t pos = 0;
        std::optional<std::string> result;

        result = str::next_chunk("abc-def-mnp-rst-uv", pos, 4);
        ASSERT_TRUE(result && (*result == "abc-") && (pos == 4));

        result = str::next_chunk("abc-def-mnp-rst-uv", pos, 4);
        ASSERT_TRUE(result && (*result == "def-") && (pos == 8));

        result = str::next_chunk("abc-def-mnp-rst-uv", pos, 4);
        ASSERT_TRUE(result && (*result == "mnp-") && (pos == 12));

        result = str::next_chunk("abc-def-mnp-rst-uv", pos, 4);
        ASSERT_TRUE(result && (*result == "rst-") && (pos == 16));

        result = str::next_chunk("abc-def-mnp-rst-uv", pos, 4);
        ASSERT_TRUE(result && (*result == "uv") && (pos == 18));

        result = str::next_chunk("abc-def-mnp-rst-uv", pos, 4);
        ASSERT_TRUE(!result && (pos == 18));

        result = str::next_chunk("abc-def-mnp-rst-uv", pos = 18, 4);
        ASSERT_TRUE(!result && (pos == 18));

        result = str::next_chunk("abc-def-mnp-rst-uv", pos = 20, 4);
        ASSERT_TRUE(!result && (pos == 18));

        result = str::next_chunk("abc-def-mnp-rst-uv", pos = str::npos, 4);
        ASSERT_TRUE(!result && (pos == 18));
    }
    SECTION("max_n") {
        size_t pos = 0;
        std::optional<std::string> result;

        // max_n = 0
        result = str::next_chunk("abc-def-mnp-rst-uv", pos = 0, 0);
        ASSERT_TRUE(result && (*result == "abc-def-mnp-rst-uv") && (pos == 18));

        // max_n = 18
        result = str::next_chunk("abc-def-mnp-rst-uv", pos = 0, 18);
        ASSERT_TRUE(result && (*result == "abc-def-mnp-rst-uv") && (pos == 18));

        // max_n = 20
        result = str::next_chunk("abc-def-mnp-rst-uv", pos = 0, 20);
        ASSERT_TRUE(result && (*result == "abc-def-mnp-rst-uv") && (pos == 18));

        // max_n = str::npos
        result = str::next_chunk("abc-def-mnp-rst-uv", pos = 0, str::npos);
        ASSERT_TRUE(result && (*result == "abc-def-mnp-rst-uv") && (pos == 18));
    }
    SECTION("空串") {
        size_t pos = 0;
        std::optional<std::string> result;

        result = str::next_chunk("", pos = 0, 0);
        ASSERT_TRUE(!result && (pos == 0));

        result = str::next_chunk("", pos = 1, 0);
        ASSERT_TRUE(!result && (pos == 0));

        result = str::next_chunk("", pos = str::npos, 0);
        ASSERT_TRUE(!result && (pos == 0));
    }
}