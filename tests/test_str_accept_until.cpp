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

TEST(test_str, accept_until) {
    GROUP("单字符为守卫符+未指定转义字符") {
        size_t pos = 0;
        std::optional<str::range_type> range;
        ASSERT_TRUE(range = str::accept_until("abc`", pos = 0, '`'));
        ASSERT_TRUE((pos == 4)&&(range == str::range(0, 3)));

        ASSERT_FALSE(range = str::accept_until("abc`", pos = 0, 'Q'));
        ASSERT_TRUE((pos == 0));

        ASSERT_TRUE(range = str::accept_until("abc`", pos = 3, '`'));
        ASSERT_TRUE((pos == 4)&&(range == str::range(3, 0)));

        ASSERT_FALSE(range = str::accept_until("abc`", pos = 4, '`'));
        ASSERT_TRUE((pos == 4));

        ASSERT_FALSE(range = str::accept_until("abc`", pos = str::npos, '`'));
        ASSERT_TRUE((pos == str::npos));
    }

    GROUP("单字符为守卫符+指定转义字符") {
        size_t pos = 0;
        std::optional<str::range_type> range;
        ASSERT_TRUE(range = str::accept_until(R"([a\]c])", pos = 0, '\\', ']'));
        ASSERT_TRUE((pos == 6) && (range == str::range(0, 5)));

        ASSERT_FALSE(range = str::accept_until(R"([a\]c)", pos = 0, '\\', ']'));
        ASSERT_TRUE((pos == 0));

        ASSERT_TRUE(range = str::accept_until(R"([a\]c])", pos = 2, '\\', ']'));
        ASSERT_TRUE((pos == 6) && (range == str::range(2, 3)));

        ASSERT_TRUE(range = str::accept_until(R"([a\]c])", pos = 3, '\\', ']'));
        ASSERT_TRUE((pos == 4) && (range == str::range(3, 0)));

        ASSERT_FALSE(range = str::accept_until(R"([a\]c])", pos = str::npos, '\\', ']'));
        ASSERT_TRUE((pos == str::npos));

        ASSERT_FALSE(range = str::accept_until(R"(\)", pos = 0, '\\', ']'));
        ASSERT_TRUE((pos == 0));

        ASSERT_FALSE(range = str::accept_until(R"(abc\)", pos = 0, '\\', ']'));
        ASSERT_TRUE((pos == 0));

        SECTION("转义字符为自身") {
            ASSERT_FALSE(range = str::accept_until(R"(ab\\c\\)", pos = 0, '\\', '\\'));
            ASSERT_TRUE((pos == 0));
        }

        SECTION("空串场景") {
            ASSERT_FALSE(range = str::accept_until("", pos = 0, '\0'));
            ASSERT_TRUE((pos == 0));

            ASSERT_FALSE(range = str::accept_until("", pos = 4, '`'));
            ASSERT_TRUE((pos == 4));

            ASSERT_FALSE(range = str::accept_until("", pos = str::npos, '`'));
            ASSERT_TRUE((pos == str::npos));
        }
    }

    GROUP("字符集做守卫符+未指定转义字符") {
        size_t pos = 0;
        std::optional<str::range_type> range;
        ASSERT_TRUE(range = str::accept_until("abc`", pos = 0, str::charset("`#%")));
        ASSERT_TRUE((pos == 4)&&(range == str::range(0, 3)));

        ASSERT_FALSE(range = str::accept_until("abc`", pos = 0, str::charset("XYZ")));
        ASSERT_TRUE((pos == 0));

        ASSERT_TRUE(range = str::accept_until("abc`", pos = 3, str::charset("`#%")));
        ASSERT_TRUE((pos == 4)&&(range == str::range(3, 0)));

        ASSERT_FALSE(range = str::accept_until("abc`", pos = 4, str::charset("`#%")));
        ASSERT_TRUE((pos == 4));

        ASSERT_FALSE(range = str::accept_until("abc`", pos = str::npos, str::charset("`#%")));
        ASSERT_TRUE((pos == str::npos));

        ASSERT_FALSE(range = str::accept_until("abc`", pos = 0, str::charset()));
        ASSERT_TRUE((pos == 0));
    }

    GROUP("字符集做守卫符+指定转义字符") {
        size_t pos = 0;
        std::optional<str::range_type> range;
        ASSERT_TRUE(range = str::accept_until(R"([a\]c])", pos = 0, '\\', str::charset("]#%")));
        ASSERT_TRUE((pos == 6) && (range == str::range(0, 5)));

        ASSERT_FALSE(range = str::accept_until(R"([a\]c)", pos = 0, '\\', str::charset("]#%")));
        ASSERT_TRUE((pos == 0));

        ASSERT_TRUE(range = str::accept_until(R"([a\]c])", pos = 2, '\\', str::charset("]#%")));
        ASSERT_TRUE((pos == 6) && (range == str::range(2, 3)));

        ASSERT_TRUE(range = str::accept_until(R"([a\]c])", pos = 3, '\\', str::charset("]#%")));
        ASSERT_TRUE((pos == 4) && (range == str::range(3, 0)));

        ASSERT_FALSE(range = str::accept_until(R"([a\]c])", pos = str::npos, '\\', str::charset("]#%")));
        ASSERT_TRUE((pos == str::npos));

        ASSERT_FALSE(range = str::accept_until(R"(\)", pos = 0, '\\', str::charset("]#%")));
        ASSERT_TRUE((pos == 0));

        ASSERT_FALSE(range = str::accept_until(R"(abc\)", pos = 0, '\\', str::charset("]#%")));
        ASSERT_TRUE((pos == 0));

        ASSERT_FALSE(range = str::accept_until(R"(abc\)", pos = 0, '\\', str::charset()));
        ASSERT_TRUE((pos == 0));

        SECTION("连续识别") {
            ASSERT_TRUE(range = str::accept_until(R"(%abc#\#def#)", pos = 0, '\\', str::charset("]#%")));
            ASSERT_TRUE((pos == 1) && (range == str::range(0, 0)));
            ASSERT_TRUE(range = str::accept_until(R"(%abc#\#def#)", pos, '\\', str::charset("]#%")));
            ASSERT_TRUE((pos == 5) && (range == str::range(1, 3)));
            ASSERT_TRUE(range = str::accept_until(R"(%abc#\#def#)", pos, '\\', str::charset("]#%")));
            ASSERT_TRUE((pos == 11) && (range == str::range(5, 5)));
            ASSERT_FALSE(range = str::accept_until(R"(%abc#\#def#)", pos, '\\', str::charset("]#%")));
            ASSERT_TRUE((pos == 11));
        }
    }

    GROUP("字符串做守卫符") {
        SECTION("连续识别") {
            size_t pos = 0;
            std::optional<str::range_type> range;
            ASSERT_TRUE(range = str::accept_until(R"(abc###def####)", pos, "##"));
            ASSERT_TRUE((pos == 5) && (range == str::range(0, 3)));
            ASSERT_TRUE(range = str::accept_until(R"(abc###def####)", pos, "##"));
            ASSERT_TRUE((pos == 11) && (range == str::range(5, 4)));
            ASSERT_TRUE(range = str::accept_until(R"(abc###def####)", pos, "##"));
            ASSERT_TRUE((pos == 13) && (range == str::range(11, 0)));
            ASSERT_FALSE(range = str::accept_until(R"(abc###def####)", pos, "##"));
            ASSERT_TRUE((pos == 13));
        }
        SECTION("未识别到") {
            size_t pos = 0;
            std::optional<str::range_type> range;
            ASSERT_FALSE(range = str::accept_until(R"(abc###def####)", pos=0, "!!"));
            ASSERT_TRUE((pos == 0));
        }
        SECTION("空串") {
            size_t pos = 0;
            std::optional<str::range_type> range;
            ASSERT_FALSE(range = str::accept_until(R"()", pos=0, "####"));
            ASSERT_TRUE((pos == 0));
            ASSERT_FALSE(range = str::accept_until(R"()", pos=1, "####"));
            ASSERT_TRUE((pos == 1));
            ASSERT_FALSE(range = str::accept_until(R"()", pos=str::npos, "####"));
            ASSERT_TRUE((pos == str::npos));
            ASSERT_FALSE(range = str::accept_until(R"()", pos=0, "####"));
            ASSERT_TRUE((pos == 0));
            ASSERT_FALSE(range = str::accept_until(R"()", pos=1, "####"));
            ASSERT_TRUE((pos == 1));
            ASSERT_FALSE(range = str::accept_until(R"()", pos=0, ""));
            ASSERT_TRUE((pos == 0));
        }
    }

    GROUP("正则表达式做守卫符") {
        size_t pos = 0;
        std::optional<str::range_type> range;
        // 0 长串
        ASSERT_FALSE(range = str::accept_until(R"(ABC0)", pos=0, std::regex("[0-9]*")));
        ASSERT_TRUE((pos == 0));
        // 有效匹配
        ASSERT_TRUE(range = str::accept_until(R"(ABC0)", pos=0, std::regex("[0-9]+")));
        ASSERT_TRUE((pos == 4) && (range == str::range(0, 3)));
        // 无任何字符匹配
        ASSERT_FALSE(range = str::accept_until(R"(ABCD)", pos=0, std::regex("[0-9]+")));
        ASSERT_TRUE((pos == 0));
        // 首字符开始匹配
        ASSERT_TRUE(range = str::accept_until(R"(0123)", pos=0, std::regex("[0-9]+")));
        ASSERT_TRUE((pos == 4) && (range == str::range(0, 0)));
        // 空串
        ASSERT_FALSE(range = str::accept_until(R"()", pos=0, std::regex("[0-9]+")));
        ASSERT_TRUE((pos == 0));
    }
}
