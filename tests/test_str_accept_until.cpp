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

TEST(test_str, accept_until) {
    size_t start{0};
    size_t pos{0};

    SECTION("一般情况：未指定转义字符") {
        ASSERT_EQ(pos = str::accept_until("abc`", start = 0, '`'), 3);
        ASSERT_EQ(start, 4);

        ASSERT_EQ(pos = str::accept_until("abc`", start = 0, 'Q'), str::npos);
        ASSERT_EQ(start, 0);

        ASSERT_EQ(pos = str::accept_until("abc`", start = 3, '`'), 3);
        ASSERT_EQ(start, 4);

        ASSERT_EQ(pos = str::accept_until("abc`", start = 4, '`'), str::npos);
        ASSERT_EQ(start, 4);

        ASSERT_EQ(pos = str::accept_until("abc`", start = str::npos, '`'), str::npos);
        ASSERT_EQ(start, str::npos);
    }

    SECTION("一般情况：指定转义字符") {
        ASSERT_EQ(pos = str::accept_until(R"([a\]c])", start = 0, ']', '\\'), 5);
        ASSERT_EQ(start, 6);

        ASSERT_EQ(pos = str::accept_until(R"([a\]c)", start = 0, 'Q', '\\'), str::npos);
        ASSERT_EQ(start, 0);

        ASSERT_EQ(pos = str::accept_until(R"([a\]c])", start = 2, ']', '\\'), 5);
        ASSERT_EQ(start, 6);

        ASSERT_EQ(pos = str::accept_until(R"([a\]c])", start = 3, ']', '\\'), 3);
        ASSERT_EQ(start, 4);

        ASSERT_EQ(pos = str::accept_until(R"([a\]c])", start = str::npos, ']', '\\'), str::npos);
        ASSERT_EQ(start, str::npos);

        ASSERT_EQ(pos = str::accept_until(R"(\)", start = 0, ']', '\\'), str::npos);
        ASSERT_EQ(start, 0);

        ASSERT_EQ(pos = str::accept_until(R"(abc\)", start = 0, ']', '\\'), str::npos);
        ASSERT_EQ(start, 0);
    }

    SECTION("转义字符为自身") {
        ASSERT_EQ(pos = str::accept_until(R"(ab\\c\\)", start = 0, '\\', '\\'), str::npos);
        ASSERT_EQ(start, 0);
    }

    SECTION("空串场景") {
        ASSERT_EQ(pos = str::accept_until("", start = 0, '\0'), str::npos);
        ASSERT_EQ(start, 0);

        ASSERT_EQ(pos = str::accept_until("", start = 4, '`'), str::npos);
        ASSERT_EQ(start, 4);
    }

    SECTION("一般情况：未指定转义字符") {
        ASSERT_EQ(pos = str::accept_until("abc`", start = 0, '`'), 3);
        ASSERT_EQ(start, 4);

        ASSERT_EQ(pos = str::accept_until("abc`", start = 0, 'Q'), str::npos);
        ASSERT_EQ(start, 0);

        ASSERT_EQ(pos = str::accept_until("abc`", start = 3, '`'), 3);
        ASSERT_EQ(start, 4);

        ASSERT_EQ(pos = str::accept_until("abc`", start = 4, '`'), str::npos);
        ASSERT_EQ(start, 4);

        ASSERT_EQ(pos = str::accept_until("abc`", start = str::npos, '`'), str::npos);
        ASSERT_EQ(start, str::npos);
    }

    SECTION("一般情况：指定转义字符") {
        ASSERT_EQ(pos = str::accept_until(R"([a\]c])", start = 0, ']', '\\'), 5);
        ASSERT_EQ(start, 6);

        ASSERT_EQ(pos = str::accept_until(R"([a\]c)", start = 0, 'Q', '\\'), str::npos);
        ASSERT_EQ(start, 0);

        ASSERT_EQ(pos = str::accept_until(R"([a\]c])", start = 2, ']', '\\'), 5);
        ASSERT_EQ(start, 6);

        ASSERT_EQ(pos = str::accept_until(R"([a\]c])", start = 3, ']', '\\'), 3);
        ASSERT_EQ(start, 4);

        ASSERT_EQ(pos = str::accept_until(R"([a\]c])", start = str::npos, ']', '\\'), str::npos);
        ASSERT_EQ(start, str::npos);

        ASSERT_EQ(pos = str::accept_until(R"(\)", start = 0, ']', '\\'), str::npos);
        ASSERT_EQ(start, 0);

        ASSERT_EQ(pos = str::accept_until(R"(abc\)", start = 0, ']', '\\'), str::npos);
        ASSERT_EQ(start, 0);
    }

    SECTION("转义字符为自身") {
        ASSERT_EQ(pos = str::accept_until(R"(ab\\c\\)", start = 0, '\\', '\\'), str::npos);
        ASSERT_EQ(start, 0);
    }

    SECTION("空串场景") {
        ASSERT_EQ(pos = str::accept_until("", start = 0, '\0'), str::npos);
        ASSERT_EQ(start, 0);

        ASSERT_EQ(pos = str::accept_until("", start = 4, '`'), str::npos);
        ASSERT_EQ(start, 4);
    }
}
