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

TEST(test_str, accept) {
    SECTION("接受单个字符") {
        size_t pos{0};
        std::optional<str::range_type> range;
        ASSERT_TRUE(range = str::accept("abc`", pos = 0, 'a'));
        ASSERT_TRUE((pos == 1) && (range == str::range(0, 1)));

        ASSERT_FALSE(range = str::accept("abc`", pos = 0, 'Q'));
        ASSERT_TRUE((pos == 0));

        ASSERT_TRUE(range = str::accept("abc`", pos = 3, '`'));
        ASSERT_TRUE((pos == 4) && (range == str::range(3, 1)));

        ASSERT_FALSE(range = str::accept("abc`", pos = 4, '`'));
        ASSERT_TRUE((pos == 4));

        ASSERT_FALSE(range = str::accept("abc`", pos = str::npos, '`'));
        ASSERT_TRUE(pos == str::npos);

        ASSERT_FALSE(range = str::accept("", pos = 0, '\0'));
        ASSERT_TRUE(pos == 0);
    }
    SECTION("接受字符串") {
        size_t pos{0};
        std::optional<str::range_type> range;

        // Full match
        ASSERT_TRUE(range = str::accept("abc`", pos = 0, "abc"));
        ASSERT_TRUE((pos == 3) && (range == str::range(0, 3)));

        // None match
        ASSERT_FALSE(range = str::accept("abc`", pos = 0, "XYZ"));
        ASSERT_TRUE((pos == 0));

        // Part match
        ASSERT_FALSE(range = str::accept("abc`", pos = 0, "abcd"));
        ASSERT_TRUE((pos == 0));

        // All string matched
        ASSERT_TRUE(range = str::accept("abc`", pos = 0, "abc`"));
        ASSERT_TRUE((pos == 4) && (range == str::range(0, 4)));

        // Need more
        ASSERT_FALSE(range = str::accept("abc`", pos = 0, "abc`QQ"));
        ASSERT_TRUE((pos == 0));

        // From EOS
        ASSERT_TRUE(range = str::accept("abc`", pos = 3, "`"));
        ASSERT_TRUE((pos == 4) && (range == str::range(3, 1)));

        // From Middle
        ASSERT_TRUE(range = str::accept("abc`", pos = 1, "bc`"));
        ASSERT_TRUE((pos == 4) && (range == str::range(1, 3)));

        // pos out of range
        ASSERT_FALSE(range = str::accept("abc`", pos = 4, "KK"));
        ASSERT_TRUE((pos == 4));

        ASSERT_FALSE(range = str::accept("abc`", pos = str::npos, "KK"));
        ASSERT_TRUE(pos == str::npos);

        // Empty string
        ASSERT_FALSE(range = str::accept("", pos = 0, "K"));
        ASSERT_TRUE(pos == 0);
        ASSERT_FALSE(range = str::accept("", pos = 1, "K"));
        ASSERT_TRUE(pos == 1);
        ASSERT_FALSE(range = str::accept("", pos = str::npos, "K"));
        ASSERT_TRUE(pos == str::npos);

        // Accept empty string
        ASSERT_FALSE(range = str::accept("", pos = 0, ""));
        ASSERT_TRUE(pos == 0);
        ASSERT_FALSE(range = str::accept("abc", pos = 1, ""));
        ASSERT_TRUE(pos == 1);
        ASSERT_FALSE(range = str::accept("", pos = str::npos, ""));
        ASSERT_TRUE(pos == str::npos);
    }
    SECTION("接受字符集合") {
        size_t pos{0};
        std::optional<str::range_type> range;
        ASSERT_TRUE(range = str::accept("abc`", pos = 0, str::charset("abc")));
        ASSERT_TRUE((pos == 3) && (range == str::range(0, 3)));

        ASSERT_TRUE(range = str::accept("abc`", pos = 0, str::charset("ac")));
        ASSERT_TRUE((pos == 1) && (range == str::range(0, 1)));

        ASSERT_FALSE(range = str::accept("abc`", pos = 0, str::charset("XYZ")));
        ASSERT_TRUE((pos == 0));

        ASSERT_TRUE(range = str::accept("abc`", pos = 0, str::charset("abcd")));
        ASSERT_TRUE((pos == 3) && (range == str::range(0, 3)));

        ASSERT_TRUE(range = str::accept("abc`", pos = 3, str::charset("`")));
        ASSERT_TRUE((pos == 4) && (range == str::range(3, 1)));

        ASSERT_FALSE(range = str::accept("abc`", pos = 4, str::charset("KK")));
        ASSERT_TRUE((pos == 4));

        ASSERT_FALSE(range = str::accept("abc`", pos = str::npos, str::charset("KK")));
        ASSERT_TRUE(pos == str::npos);

        ASSERT_FALSE(range = str::accept("abc`", pos = 0, str::charset()));
        ASSERT_TRUE(pos == 0);
    }
    SECTION("接受正则表达式") {
        size_t pos{0};
        std::optional<str::range_type> range;
        ASSERT_TRUE(range = str::accept("abc`", pos = 0, std::regex("[a-z]+")));
        ASSERT_TRUE((pos == 3) && (range == str::range(0, 3)));

        ASSERT_TRUE(range = str::accept("abc`", pos = 0, std::regex("[a-z]+`")));
        ASSERT_TRUE((pos == 4) && (range == str::range(0, 4)));

        ASSERT_FALSE(range = str::accept("abc`", pos = 0, std::regex("[X-Z]+")));
        ASSERT_TRUE((pos == 0));

        ASSERT_FALSE(range = str::accept("abc`", pos = 0, std::regex("abcd")));
        ASSERT_TRUE((pos == 0));

        ASSERT_FALSE(range = str::accept("abc`", pos = 0, std::regex("[bc]")));
        ASSERT_TRUE((pos == 0));

        ASSERT_TRUE(range = str::accept("abc```", pos = 3, std::regex("`{1,2}")));
        ASSERT_TRUE((pos == 5) && (range == str::range(3, 2)));

        ASSERT_FALSE(range = str::accept("abc`", pos = 4, std::regex("K+")));
        ASSERT_TRUE((pos == 4));

        ASSERT_FALSE(range = str::accept("abc`", pos = str::npos, std::regex("K+")));
        ASSERT_TRUE(pos == str::npos);

        ASSERT_FALSE(range = str::accept("abc`", pos = 0, std::regex("K*")));
        ASSERT_TRUE(pos == 0);

        ASSERT_TRUE(range = str::accept("abc`", pos = 1, std::regex("[a-z]*K*")));
        ASSERT_TRUE((pos == 3) && (range == str::range(1, 2)));

        ASSERT_TRUE(range = str::accept("abcKKK", pos = 2, std::regex("[a-z]*[A-Z]*")));
        ASSERT_TRUE((pos == 6) && (range == str::range(2, 4)));
    }
}
