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

TEST(test_str, count) {
    SECTION("检查是否匹配某个正则表达式") {
        ASSERT_EQ(str::count("3bc12def1233", "12"), 2);
        ASSERT_EQ(str::count("3bc12def1233", "3"), 3);
        ASSERT_EQ(str::count("3bc12def1233", "A"), 0);
        ASSERT_EQ(str::count("3bc12def1233", "def"), 1);
    }
    SECTION("统计子串数量") {
        ASSERT_EQ(str::count("333", "33"), 1);   // 不会考虑重问题
        ASSERT_EQ(str::count("33333", "33"), 2); // 不会考虑重问题
        ASSERT_EQ(str::count("333433343333334", "33"), 5);
        ASSERT_EQ(str::count("333433343333334", ""), 16);
        ASSERT_EQ(str::count("333433343333334", "333"), 4);
    }
    SECTION("统计单字符") {
        ASSERT_EQ(str::count("3bc12def1233", '3'), 3);
        ASSERT_EQ(str::count("3bc12def1233", '\n'), 0);
        ASSERT_EQ(str::count(std::string_view{"3bc12def\0\0#1233", 15}, '\0'), 2);
        ASSERT_EQ(str::count("3bc12def\0\0#1233", '\0'), 0);
    }
    SECTION("空串") {
        ASSERT_EQ(str::count("", ""), 1);
        ASSERT_EQ(str::count("ABCDE", ""), 6);
        ASSERT_EQ(str::count("", "ABCDE"), 0);
    }
    SECTION("指定字符范围") {
        ASSERT_EQ(str::count("3bc12def1233", [](str::value_type ch) -> bool {
            return std::isalpha(ch);
        }),
            5);
        ASSERT_EQ(str::count("3bc12def1233", [](str::value_type ch) -> bool {
            return std::isalnum(ch);
        }),
            12);
        ASSERT_EQ(str::count("3bc12def1233", [](str::value_type ch) -> bool {
            return std::isupper(ch);
        }),
            0);
    }
    SECTION("指定字符集") {
        ASSERT_EQ(str::count("3bc12def1233", str::charset_type{"abcd"}), 3);
        ASSERT_EQ(str::count("3bc12def1233", str::charset_type{}), 0);
        ASSERT_EQ(str::count("", str::charset_type{}), 0);
    }
    SECTION("指定正则") {
        ASSERT_EQ(str::count("3bc12def1233", std::regex{"[a-z]+"}), 2);
        ASSERT_EQ(str::count("3bc12def1233", std::regex{"[a-z]*"}), 10);
        ASSERT_EQ(str::count("3bc12def1233", std::regex{"[A-Za-z][0-9]*"}), 5);
        ASSERT_EQ(str::count("123", std::regex{"[A-Za-z][0-9]*"}), 0);
        ASSERT_EQ(str::count("", str::charset_type{".*"}), 0);
    }
}