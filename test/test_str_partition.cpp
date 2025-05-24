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
#include "test-compares.hpp"
#include "test-to_proc.hpp"

TEST(test_str, partition) {
    SECTION("以字符串分割") {
        ASSERT_EQ(str::partition("aaa::bbb", "::"), (std::array{"aaa", "::", "bbb"}));
        ASSERT_EQ(str::partition("::bbb", "::"), (std::array{"", "::", "bbb"}));
        ASSERT_EQ(str::partition("aaa::", "::"), (std::array{"aaa", "::", ""}));
        ASSERT_EQ(str::partition("::", "::"), (std::array{"", "::", ""}));
        ASSERT_EQ(str::partition("abc", "::"), (std::array{"abc", "", ""}));
        ASSERT_EQ(str::partition("", "::"), (std::array{"", "", ""}));
        ASSERT_EQ(str::partition("aaa::bbb", ""), (std::array{"aaa::bbb", "", ""}));
        ASSERT_EQ(str::partition("", ""), (std::array{"", "", ""}));
    }
    SECTION("以字符集分割") {
        ASSERT_EQ(str::partition("aaa::bbb", str::charset(";:")), (std::array{"aaa", ":", ":bbb"}));
        ASSERT_EQ(str::partition("aaa::bbb", str::charset("*/")), (std::array{"aaa::bbb", "", ""}));
        ASSERT_EQ(str::partition(",bbb", str::charset(",")), (std::array{"", ",", "bbb"}));
        ASSERT_EQ(str::partition("aaa,", str::charset(",")), (std::array{"aaa", ",", ""}));
        ASSERT_EQ(str::partition(":", str::charset(":")), (std::array{"", ":", ""}));
        ASSERT_EQ(str::partition("", str::charset(",")), (std::array{"", "", ""}));
        ASSERT_EQ(str::partition("abc", str::charset("")), (std::array{"abc", "", ""}));
        ASSERT_EQ(str::partition("", str::charset("")), (std::array{"", "", ""}));
    }
    SECTION("以char-proc分割") {
        ASSERT_EQ(str::partition("aaa::bbb", [](str::value_type ch) -> int { return (ch == ':') || (ch == ';'); }), (std::array{"aaa", ":", ":bbb"}));
        ASSERT_EQ(str::partition("aaa::bbb", [](str::value_type ch) -> int { return (ch == '*') || (ch == '/'); }), (std::array{"aaa::bbb", "", ""}));
        ASSERT_EQ(str::partition(",bbb", [](str::value_type ch) -> int { return (ch == ','); }), (std::array{"", ",", "bbb"}));
        ASSERT_EQ(str::partition("aaa,", [](str::value_type ch) -> int { return (ch == ','); }), (std::array{"aaa", ",", ""}));
        ASSERT_EQ(str::partition(":", [](str::value_type ch) -> int { return (ch == ':'); }), (std::array{"", ":", ""}));
        ASSERT_EQ(str::partition("", [](str::value_type ch) -> int { return (ch == ','); }), (std::array{"", "", ""}));
        ASSERT_EQ(str::partition("abc", [](str::value_type) -> int { return false; }), (std::array{"abc", "", ""}));
        ASSERT_EQ(str::partition("", [](str::value_type) -> int { return false; }), (std::array{"", "", ""}));
    }
    SECTION("view-proc分割") {
        ASSERT_EQ(str::partition("aaa::bbb", to_search_proc{"::"}), (std::array{"aaa", "::", "bbb"}));
        ASSERT_EQ(str::partition("aaa::bbb", to_search_proc{"||"}), (std::array{"aaa::bbb", "", ""}));
        ASSERT_EQ(str::partition(",,bbb", to_search_proc{",,"}), (std::array{"", ",,", "bbb"}));
        ASSERT_EQ(str::partition("aaa,,", to_search_proc{",,"}), (std::array{"aaa", ",,", ""}));
        ASSERT_EQ(str::partition("::", to_search_proc{"::"}), (std::array{"", "::", ""}));
        ASSERT_EQ(str::partition("", to_search_proc{"##"}), (std::array{"", "", ""}));
        ASSERT_EQ(str::partition("abc", [](std::string_view) -> std::optional<std::string> { return std::nullopt; }), (std::array{"abc", "", ""}));
        ASSERT_EQ(str::partition("", [](std::string_view) -> std::optional<std::string> { return std::nullopt; }), (std::array{"", "", ""}));
    }
}
