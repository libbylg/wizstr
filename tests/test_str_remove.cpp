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

TEST(test_str, remove) {
    SECTION("一般情况") {
        ASSERT_EQ(str::append("aaa", "bbb"), "aaabbb");
    }
    SECTION("空串") {
        ASSERT_EQ(str::append("aaa", ""), "aaa");
        ASSERT_EQ(str::append("", "aaa"), "aaa");
        ASSERT_EQ(str::append("", ""), "");
    }
    // SECTION("追加字符") {
    //     ASSERT_EQ(str::append("aaa", 'A'), "aaaA");
    //     ASSERT_EQ(str::append("", 'A'), "A");
    //     ASSERT_EQ(str::append("", '\0'), (std::string_view{"\0", 1}));
    // }
    // SECTION("追加重复的字符") {
    //     ASSERT_EQ(str::append("aaa", 'A', 3), "aaaAAA");
    //     ASSERT_EQ(str::append("", 'A', 3), "AAA");
    //     ASSERT_EQ(str::append("", '\0', 3), (std::string_view{"\0\0\0", 3}));
    // }
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
