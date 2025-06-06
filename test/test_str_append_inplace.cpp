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

#include <list>

TEST(test_str, append_inplace) {
    SECTION("一般情况") {
        std::string s{"abc"};
        ASSERT_EQ(str::append_inplace(s, "def"), "abcdef");
        ASSERT_EQ(str::append_inplace(s, "X", 3), "abcdefXXX");
        ASSERT_EQ(s, "abcdefXXX");
    }
    SECTION("空串") {
        std::string s{""};
        ASSERT_EQ(str::append_inplace(s, "", 5), "");
        ASSERT_EQ(str::append_inplace(s, "def"), "def");
        ASSERT_EQ(str::append_inplace(s, ""), "def");
        ASSERT_EQ(str::append_inplace(s, "X", 3), "defXXX");
        ASSERT_EQ(s, "defXXX");
    }
    SECTION("proc模式") {
        std::string s{"123"};
        std::array<std::string, 5> items{
            "ABC",
            "",
            "xyz"
            "K",
            "\n"};
        auto itr = items.cbegin();
        auto end = items.cend();
        ASSERT_EQ(str::append_inplace(s, [&itr, &end]() -> std::optional<std::string_view> {
            if (itr == end) {
                return std::nullopt;
            }

            return *(itr++);
        }),
            "123ABCxyzK\n");
        ASSERT_EQ(s, "123ABCxyzK\n");
    }
    SECTION("proc模式:立即返回结束") {
        std::string s{"123"};
        ASSERT_EQ(str::append_inplace(s, []() -> std::optional<std::string_view> {
            return std::nullopt;
        }),
            "123");
        ASSERT_EQ(s, "123");
    }
    SECTION("直接添加一个序列:std::list<xxx>") {
        std::string s{"123"};
        std::list<std::string_view> items{
            "ABC",
            "",
            "xyz"
            "K",
            "PP"};
        ASSERT_EQ(str::append_inplace(s, items), "123ABCxyzKPP");
        ASSERT_EQ(s, "123ABCxyzKPP");
    }
    SECTION("直接添加一个序列:std::array<xxx>") {
        std::string s{"123"};
        std::array<std::string, 5> items{
            "ABC",
            "",
            "xyz"
            "K",
            "PP"};
        ASSERT_EQ(str::append_inplace(s, items), "123ABCxyzKPP");
        ASSERT_EQ(s, "123ABCxyzKPP");
    }
}
