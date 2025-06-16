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

TEST(test_str, insert) {
    SECTION("一般情况") {
        ASSERT_EQ(str::insert("aaa", 0, "bbb"), "bbbaaa");
        ASSERT_EQ(str::insert("aaa", 1, "bbb"), "abbbaa");
        ASSERT_EQ(str::insert("aaa", 2, "bbb"), "aabbba");
        ASSERT_EQ(str::insert("aaa", 3, "bbb"), "aaabbb");
        ASSERT_EQ(str::insert("aaa", 4, "bbb"), "aaabbb");
        ASSERT_EQ(str::insert("aaa", str::npos, "bbb"), "aaabbb");
        ASSERT_EQ(str::insert("aaa", 1, "bbb", 3), "abbbbbbbbbaa");
        ASSERT_EQ(str::insert("aaa", 1, "bbb", 0), "aaa");
        ASSERT_EQ(str::insert("aaa", 1, "", 99), "aaa");
    }
    SECTION("插入空串") {
        ASSERT_EQ(str::insert("aaa", 0, ""), "aaa");
        ASSERT_EQ(str::insert("aaa", 1, ""), "aaa");
        ASSERT_EQ(str::insert("aaa", 2, ""), "aaa");
        ASSERT_EQ(str::insert("aaa", 3, ""), "aaa");
        ASSERT_EQ(str::insert("aaa", 4, ""), "aaa");
        ASSERT_EQ(str::insert("aaa", str::npos, ""), "aaa");
    }
    SECTION("插入到空串") {
        ASSERT_EQ(str::insert("", 0, ""), ""); // 将空串插入到空串
        ASSERT_EQ(str::insert("", 0, "bbb"), "bbb");
        ASSERT_EQ(str::insert("", 1, "bbb"), "bbb");
        ASSERT_EQ(str::insert("", str::npos, "bbb"), "bbb");
    }
    SECTION("插入单个字符") {
        ASSERT_EQ(str::insert("aaa", 0, 'B'), "Baaa");
        ASSERT_EQ(str::insert("aaa", 1, 'B'), "aBaa");
        ASSERT_EQ(str::insert("aaa", 2, 'B'), "aaBa");
        ASSERT_EQ(str::insert("aaa", 3, 'B'), "aaaB");
        ASSERT_EQ(str::insert("aaa", 4, 'B'), "aaaB");
        ASSERT_EQ(str::insert("aaa", str::npos, 'B'), "aaaB");
        ASSERT_EQ(str::insert("", 0, 'B'), "B");
        ASSERT_EQ(str::insert("", 9, 'B'), "B");
    }
    SECTION("重复插入一个字符") {
        ASSERT_EQ(str::insert("aaa", 0, 'B', 3), "BBBaaa");
        ASSERT_EQ(str::insert("aaa", 1, 'B', 3), "aBBBaa");
        ASSERT_EQ(str::insert("aaa", 2, 'B', 3), "aaBBBa");
        ASSERT_EQ(str::insert("aaa", 3, 'B', 3), "aaaBBB");
        ASSERT_EQ(str::insert("aaa", 4, 'B', 3), "aaaBBB");
        ASSERT_EQ(str::insert("aaa", str::npos, 'B', 3), "aaaBBB");
        ASSERT_EQ(str::insert("", 0, 'B', 3), "BBB");
        ASSERT_EQ(str::insert("", 9, 'B', 3), "BBB");
        ASSERT_EQ(str::insert("", 0, 'B', 0), "");
        ASSERT_EQ(str::insert("aaa", 2, 'B', 0), "aaa");
    }
    SECTION("通过proc提供数据:一般") {
        std::vector<std::string_view> items{
            "Hello",
            "World",
            "",
            "!!!",
        };
        str::size_type index = 0;
        std::string result = str::insert("abc", 2, [&items, &index]() -> std::optional<std::string_view> {
            if (index >= items.size()) {
                return std::nullopt;
            }

            return items[index++];
        });
        ASSERT_EQ(result, "abHelloWorld!!!c");
    }
    SECTION("通过proc提供数据:pos超出范围1") {
        std::vector<std::string_view> items{
            "Hello",
            "World",
            "",
            "!!!",
        };
        str::size_type index = 0;
        std::string result = str::insert("abc", 5, [&items, &index]() -> std::optional<std::string_view> {
            if (index >= items.size()) {
                return std::nullopt;
            }

            return items[index++];
        });
        ASSERT_EQ(result, "abcHelloWorld!!!");
    }
    SECTION("通过proc提供数据:pos超出范围2") {
        std::vector<std::string_view> items{
            "Hello",
            "World",
            "",
            "!!!",
        };
        str::size_type index = 0;
        std::string result = str::insert("abc", str::npos, [&items, &index]() -> std::optional<std::string_view> {
            if (index >= items.size()) {
                return std::nullopt;
            }

            return items[index++];
        });
        ASSERT_EQ(result, "abcHelloWorld!!!");
    }
    SECTION("通过proc提供数据:立即终止") {
        std::string result = str::insert("abc", 2, []() -> std::optional<std::string_view> {
            return std::nullopt;
        });
        ASSERT_EQ(result, "abc");
    }
}