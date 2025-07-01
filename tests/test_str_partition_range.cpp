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

TEST(test_str, partition_range) {
    SECTION("以字符串分割") {
        ASSERT_EQ(str::partition_range("aaa::bbb", "::"), (std::array{str::range(0, 3), str::range(3, 2), str::range(5, 3)}));
        ASSERT_EQ(str::partition_range("::bbb", "::"), (std::array{str::range(0, 0), str::range(0, 2), str::range(2, 3)}));
        ASSERT_EQ(str::partition_range("aaa::", "::"), (std::array{str::range(0, 3), str::range(3, 2), str::range(5, 0)}));
        ASSERT_EQ(str::partition_range("::", "::"), (std::array{str::range(0, 0), str::range(0, 2), str::range(2, 0)}));
        ASSERT_EQ(str::partition_range("abc", "::"), (std::array{str::range(0, 3), str::range(3, 0), str::range(3, 0)}));
        ASSERT_EQ(str::partition_range("", "::"), (std::array{str::range(0, 0), str::range(0, 0), str::range(0, 0)}));
        ASSERT_EQ(str::partition_range("aaa::bbb", ""), (std::array{str::range(0, 8), str::range(8, 0), str::range(8, 0)}));
        ASSERT_EQ(str::partition_range("", ""), (std::array{str::range(0, 0), str::range(0, 0), str::range(0, 0)}));
    }
    SECTION("以字符集分割") {
        ASSERT_EQ(str::partition_range("aaa::bbb", str::charset(";:")), (std::array{str::range(0, 3), str::range(3, 1), str::range(4, 4)}));
        ASSERT_EQ(str::partition_range("aaa::bbb", str::charset("*/")), (std::array{str::range(0, 8), str::range(8, 0), str::range(8, 0)}));
        ASSERT_EQ(str::partition_range(",bbb", str::charset(",")), (std::array{str::range(0, 0), str::range(0, 1), str::range(1, 3)}));
        ASSERT_EQ(str::partition_range("aaa,", str::charset(",")), (std::array{str::range(0, 3), str::range(3, 1), str::range(4, 0)}));
        ASSERT_EQ(str::partition_range(":", str::charset(":")), (std::array{str::range(0, 0), str::range(0, 1), str::range(1, 0)}));
        ASSERT_EQ(str::partition_range("", str::charset(",")), (std::array{str::range(0, 0), str::range(0, 0), str::range(0, 0)}));
        ASSERT_EQ(str::partition_range("abc", str::charset("")), (std::array{str::range(0, 3), str::range(3, 0), str::range(3, 0)}));
        ASSERT_EQ(str::partition_range("", str::charset("")), (std::array{str::range(0, 0), str::range(0, 0), str::range(0, 0)}));
    }
    SECTION("以char-proc分割") {
        // clang-format off
        ASSERT_EQ(str::partition_range("aaa::bbb", [](str::value_type ch) -> int { return (ch == ':') || (ch == ';'); }), (std::array{str::range(0, 3), str::range(3, 1), str::range(4, 4)}));
        ASSERT_EQ(str::partition_range("aaa::bbb", [](str::value_type ch) -> int { return (ch == '*') || (ch == '/'); }), (std::array{str::range(0, 8), str::range(8, 0), str::range(0, 0)}));
        ASSERT_EQ(str::partition_range(",bbb", [](str::value_type ch) -> int { return (ch == ','); }), (std::array{str::range(0, 0), str::range(0, 1), str::range(1, 3)}));
        ASSERT_EQ(str::partition_range("aaa,", [](str::value_type ch) -> int { return (ch == ','); }), (std::array{str::range(0, 3), str::range(3, 1), str::range(4, 0)}));
        ASSERT_EQ(str::partition_range(":", [](str::value_type ch) -> int { return (ch == ':'); }), (std::array{str::range(0, 0), str::range(0, 1), str::range(1, 0)}));
        ASSERT_EQ(str::partition_range("", [](str::value_type ch) -> int { return (ch == ','); }), (std::array{str::range(0, 0), str::range(0, 0), str::range(0, 0)}));
        ASSERT_EQ(str::partition_range("abc", [](str::value_type) -> int { return false; }), (std::array{str::range(0, 3), str::range(3, 0), str::range(3, 0)}));
        ASSERT_EQ(str::partition_range("", [](str::value_type) -> int { return false; }), (std::array{str::range(0, 0), str::range(0, 0), str::range(0, 0)}));
        // clang-format on
    }
    // SECTION("view-proc分割") {
    //     ASSERT_EQ(str::partition_range("aaa::bbb", to_search_proc{"::"}), (std::array{"aaa", "::", "bbb"}));
    //     ASSERT_EQ(str::partition_range("aaa::bbb", to_search_proc{"||"}), (std::array{"aaa::bbb", "", ""}));
    //     ASSERT_EQ(str::partition_range(",,bbb", to_search_proc{",,"}), (std::array{"", ",,", "bbb"}));
    //     ASSERT_EQ(str::partition_range("aaa,,", to_search_proc{",,"}), (std::array{"aaa", ",,", ""}));
    //     ASSERT_EQ(str::partition_range("::", to_search_proc{"::"}), (std::array{"", "::", ""}));
    //     ASSERT_EQ(str::partition_range("", to_search_proc{"##"}), (std::array{"", "", ""}));
    //     ASSERT_EQ(str::partition_range("abc", [](std::string_view) -> std::optional<std::string> { return std::nullopt; }), (std::array{"abc", "", ""}));
    //     ASSERT_EQ(str::partition_range("", [](std::string_view) -> std::optional<std::string> { return std::nullopt; }), (std::array{"", "", ""}));
    // }
}
