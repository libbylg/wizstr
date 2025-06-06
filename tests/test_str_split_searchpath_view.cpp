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

#include "test-utils.hpp"

TEST(test_str, split_searchpath_view) {
    SECTION("空串") {
        ASSERT_EQ(str::split_searchpath_view(""), (std::vector<std::string_view>{}));
        ASSERT_EQ(str::split_searchpath_view("", true), (std::vector<std::string_view>{""}));
    }
    SECTION("空路径") {
        ASSERT_EQ(str::split_searchpath_view(":::"), (std::vector<std::string_view>{}));
        ASSERT_EQ(str::split_searchpath_view(":::", true), (std::vector<std::string_view>{"", "", "", ""}));

        ASSERT_EQ(str::split_searchpath_view(":  : :"), (std::vector<std::string_view>{"  ", " "}));
        ASSERT_EQ(str::split_searchpath_view(":  : :", true), (std::vector<std::string_view>{"", "  ", " ", ""}));

        ASSERT_EQ(str::split_searchpath_view(":aaa", false), (std::vector<std::string_view>{"aaa"}));
        ASSERT_EQ(str::split_searchpath_view(":aaa", true), (std::vector<std::string_view>{"", "aaa"}));

        ASSERT_EQ(str::split_searchpath_view("abc:def:hij", false), (std::vector<std::string_view>{"abc", "def", "hij"}));
        ASSERT_EQ(str::split_searchpath_view("abc:def:hij", true), (std::vector<std::string_view>{"abc", "def", "hij"}));

        ASSERT_EQ(str::split_searchpath_view("abc::hij"), (std::vector<std::string_view>{"abc", "hij"}));
        ASSERT_EQ(str::split_searchpath_view("abc::hij", true), (std::vector<std::string_view>{"abc", "", "hij"}));
    }
}
