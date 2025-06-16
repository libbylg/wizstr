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

#include "test-utils.hpp"

#include <list>

#if defined(STR_NAMESPACE)
using str = STR_NAMESPACE::str;
#endif

TEST(test_str, join_searchpath) {
    SECTION("容器+默认sep") {
        ASSERT_EQ(str::join_searchpath({"A", "B", "C"}), "A:B:C");
        ASSERT_EQ(str::join_searchpath({"A", "B"}), "A:B");
        ASSERT_EQ(str::join_searchpath({"A"}), "A");
        ASSERT_EQ(str::join_searchpath({}), "");
        ASSERT_EQ(str::join_searchpath({""}), "");
        ASSERT_EQ(str::join_searchpath({"", ""}), "");
        ASSERT_EQ(str::join_searchpath({"", "", "AAA"}), "AAA");
        ASSERT_EQ(str::join_searchpath({"", "A", "B", "", "C", ""}), "A:B:C");
    }
    SECTION("容器+指定sep") {
        ASSERT_EQ(str::join_searchpath(";", {"A", "B", "C"}), "A;B;C");
        ASSERT_EQ(str::join_searchpath(";", {"A", "B"}), "A;B");
        ASSERT_EQ(str::join_searchpath(";", {"A"}), "A");
        ASSERT_EQ(str::join_searchpath(";", {}), "");
        ASSERT_EQ(str::join_searchpath(";", {""}), "");
        ASSERT_EQ(str::join_searchpath(";", {"", ""}), "");
        ASSERT_EQ(str::join_searchpath(";", {"", "", "AAA"}), "AAA");
        ASSERT_EQ(str::join_searchpath(";", {"", "A", "B", "", "C", ""}), "A;B;C");
    }
    SECTION("proc+默认sep") {
        ASSERT_EQ(str::join_searchpath(to_proc{std::list{"A", "B", "C"}}), "A:B:C");
        ASSERT_EQ(str::join_searchpath(to_proc{std::list{"A", "B"}}), "A:B");
        ASSERT_EQ(str::join_searchpath(to_proc{std::list{"A"}}), "A");
        ASSERT_EQ(str::join_searchpath(to_proc{std::list<std::string>{}}), "");
        ASSERT_EQ(str::join_searchpath(to_proc{std::list{""}}), "");
        ASSERT_EQ(str::join_searchpath(to_proc{std::list{"", ""}}), "");
        ASSERT_EQ(str::join_searchpath(to_proc{std::list{"", "", "AAA"}}), "AAA");
        ASSERT_EQ(str::join_searchpath(to_proc{std::list{"", "A", "B", "", "C", ""}}), "A:B:C");
    }
    SECTION("proc+指定sep") {
        ASSERT_EQ(str::join_searchpath("#", to_proc{std::list{"A", "B", "C"}}), "A#B#C");
        ASSERT_EQ(str::join_searchpath("#", to_proc{std::list{"A", "B"}}), "A#B");
        ASSERT_EQ(str::join_searchpath("#", to_proc{std::list{"A"}}), "A");
        ASSERT_EQ(str::join_searchpath("#", to_proc{std::list<std::string>{}}), "");
        ASSERT_EQ(str::join_searchpath("#", to_proc{std::list{""}}), "");
        ASSERT_EQ(str::join_searchpath("#", to_proc{std::list{"", ""}}), "");
        ASSERT_EQ(str::join_searchpath("#", to_proc{std::list{"", "", "AAA"}}), "AAA");
        ASSERT_EQ(str::join_searchpath("#", to_proc{std::list{"", "A", "B", "", "C", ""}}), "A#B#C");
    }
}
