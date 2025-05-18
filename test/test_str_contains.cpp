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

TEST(test_str, contains) {
    SECTION("简单场景") {
        ASSERT_EQ(str::contains("3bc1233", "3bc"), true);
        ASSERT_EQ(str::contains("3bc1233", "12"), true);
        ASSERT_EQ(str::contains("bc1233", "33"), true);
        ASSERT_EQ(str::contains("bc1233", "31"), false);
        ASSERT_EQ(str::contains("bc1233", "bc1233"), true);
    }
    SECTION("空串") {
        ASSERT_EQ(str::contains("", ""), true);
        ASSERT_EQ(str::contains("abc", ""), true);
        ASSERT_EQ(str::contains("", "abc"), false);
    }
    SECTION("针对字符") {
        ASSERT_EQ(str::contains(std::string_view{"abc\0def", 7}, '\0'), true); // 指定长度时，\0 被包含在内
        ASSERT_EQ(str::contains("abc", '\0'),
                  false);                          // "abc" 转换为 std::string_view 时，并不会包含 \0 字符
        ASSERT_EQ(str::contains("abc", 'a'), true);
        ASSERT_EQ(str::contains("abc", 'b'), true);
        ASSERT_EQ(str::contains("abc", 'c'), true);
        ASSERT_EQ(str::contains("abc", 'K'), false);
    }
    SECTION("针对字符集:是否包含字符集的任意一个") {
        ASSERT_EQ(str::contains("abc", str::charset("a")), true);
        ASSERT_EQ(str::contains("abc", str::charset("abcK")), true);
        ASSERT_EQ(str::contains("abc", str::charset("abc")), true);
        ASSERT_EQ(str::contains("abc", str::charset("ABC")), false);
        ASSERT_EQ(str::contains("abc", str::charset("")), false);
    }
    SECTION("针对正则表达式") {
        ASSERT_EQ(str::contains("abc123", std::regex("[0-9]{2}")), true);
        ASSERT_EQ(str::contains("abc123", std::regex("[0-9]{4}")), false);
        ASSERT_EQ(str::contains("abc123", std::regex("[A-Z]")), false);
        ASSERT_EQ(str::contains("12ab", std::regex("[a-z]*")), true);
        ASSERT_EQ(str::contains("12", std::regex("[a-z]*")), true);
        ASSERT_EQ(str::contains("", std::regex("[a-z]*")), true);
        ASSERT_EQ(str::contains("", std::regex(".*")), true);
    }
    SECTION("针对proc") {
        ASSERT_EQ(str::contains("abc123", [](str::value_type ch) -> bool {
            return std::isalpha(ch);
        }), true);
        ASSERT_EQ(str::contains("abc", [](str::value_type ch) -> bool {
            return std::isupper(ch);
        }), false);
    }
}
