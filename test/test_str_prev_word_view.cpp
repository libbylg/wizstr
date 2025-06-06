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

TEST(test_str, prev_word_view) {
    SECTION("不同起始位置") {
        str::size_type pos = 0;
        ASSERT_EQ(str::prev_word_view("abc def", pos = 0), std::string(""));
        ASSERT_EQ(pos, 0);
        ASSERT_EQ(str::prev_word_view("abc def", pos = 1), std::string("a"));
        ASSERT_EQ(pos, 0);
        ASSERT_EQ(str::prev_word_view("abc def", pos = 2), std::string("ab"));
        ASSERT_EQ(pos, 0);
        ASSERT_EQ(str::prev_word_view("abc def", pos = 3), std::string("abc"));
        ASSERT_EQ(pos, 0);
        ASSERT_EQ(str::prev_word_view("abc def", pos = 4), std::string("abc"));
        ASSERT_EQ(pos, 0);
        ASSERT_EQ(str::prev_word_view("abc def", pos = 5), std::string("d"));
        ASSERT_EQ(pos, 4);
        ASSERT_EQ(str::prev_word_view("abc def", pos = 6), std::string("de"));
        ASSERT_EQ(pos, 4);
        ASSERT_EQ(str::prev_word_view("abc def", pos = 7), std::string("def"));
        ASSERT_EQ(pos, 4);
        ASSERT_EQ(str::prev_word_view("abc def", pos = str::npos), std::string("def"));
        ASSERT_EQ(pos, 4);
    }
    SECTION("不同空格和多段字符串组合") {
        str::size_type pos = 0;

        ASSERT_EQ(str::prev_word_view(" \r\t\n abc \r\t\n def \r\t\n ", pos = 0), std::string(""));
        ASSERT_EQ(pos, 0);
        ASSERT_EQ(str::prev_word_view(" \r\t\n abc \r\t\n def \r\t\n ", pos = 8), std::string("abc"));
        ASSERT_EQ(pos, 5);
        ASSERT_EQ(str::prev_word_view(" \r\t\n abc \r\t\n def \r\t\n ", pos = 16), std::string("def"));
        ASSERT_EQ(pos, 13);
    }
    SECTION("无任何空白的串") {
        str::size_type pos = 0;
        ASSERT_EQ(str::prev_word_view("abc", pos = 0), std::string(""));
        ASSERT_EQ(pos, 0);
        ASSERT_EQ(str::prev_word_view("abcdef", pos = str::npos), std::string("abcdef"));
        ASSERT_EQ(pos, 0);
    }
    SECTION("空串") {
        str::size_type pos = 0;
        ASSERT_EQ(str::prev_word_view("", pos = 0), std::string(""));
        ASSERT_EQ(pos, 0);
        ASSERT_EQ(str::prev_word_view("", pos = str::npos), std::string(""));
        ASSERT_EQ(pos, 0);
    }
    SECTION("全空白串") {
        str::size_type pos = 0;
        ASSERT_EQ(str::prev_word_view(" \r\t\n ", pos = 1), std::string(""));
        ASSERT_EQ(pos, 0);
    }
}
