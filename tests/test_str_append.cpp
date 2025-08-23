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

#include <list>

#if defined(WIZSTR_NAMESPACE)
using str = WIZSTR_NAMESPACE::str;
#endif

TEST(test_str, append) {
    SECTION("一般情况") {
        ASSERT_EQ(str::append("aaa", "bbb"), "aaabbb");
        ASSERT_EQ(str::append("aaa", "bbb", 0), "aaa");
        ASSERT_EQ(str::append("aaa", "bbb", 3), "aaabbbbbbbbb");
    }
    SECTION("空串") {
        ASSERT_EQ(str::append("aaa", ""), "aaa");
        ASSERT_EQ(str::append("aaa", "", 3), "aaa");
        ASSERT_EQ(str::append("", "aaa"), "aaa");
        ASSERT_EQ(str::append("", "aaa", 0), "");
        ASSERT_EQ(str::append("", ""), "");
        ASSERT_EQ(str::append("", "", 3), "");
    }
    SECTION("给定序列") {
        ASSERT_EQ(str::append("aaa", {""}), "aaa");
        ASSERT_EQ(str::append("aaa", {"", "def"}), "aaadef");
        ASSERT_EQ(str::append("aaa", std::array{"mmm", "nnn"}), "aaammmnnn");
        ASSERT_EQ(str::append("aaa", std::vector<std::string>{"", "nnn"}), "aaannn");
        ASSERT_EQ(str::append("", {}), "");
    }
    SECTION("通过proc提供数据:一般") {
        std::vector<std::string_view> items{
            "Hello",
            "World",
            "",
            "!!!",
        };
        str::size_type index = 0;
        std::string result = str::append("abc", [&items, &index]() -> std::optional<std::string_view> {
            if (index >= items.size()) {
                return std::nullopt;
            }

            return items[index++];
        });
        ASSERT_EQ(result, "abcHelloWorld!!!");
    }
    SECTION("通过proc提供数据:立即终止") {
        std::string result = str::append("abc", []() -> std::optional<std::string_view> {
            return std::nullopt;
        });
        ASSERT_EQ(result, "abc");
    }
}
