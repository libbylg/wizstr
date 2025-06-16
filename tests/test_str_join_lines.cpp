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

#include <array>
#include <list>
#include <vector>

#if defined(STR_NAMESPACE)
using str = STR_NAMESPACE::str;
#endif

TEST(test_str, join_lines) {
    SECTION("容器模式:不带line_ends参数") {
        ASSERT_EQ(str::join_lines({"A", "B", "C"}), "A\nB\nC\n");
        ASSERT_EQ(str::join_lines(std::vector{"A", "B", "C"}), "A\nB\nC\n");
        ASSERT_EQ(str::join_lines(std::list{"A", "B"}), "A\nB\n");
        ASSERT_EQ(str::join_lines(std::array{"A"}), "A\n");
        ASSERT_EQ(str::join_lines({}), "");
        ASSERT_EQ(str::join_lines(std::list{"A", "", "C"}), "A\n\nC\n");
        ASSERT_EQ(str::join_lines(std::array{"", "", ""}), "\n\n\n");
        ASSERT_EQ(str::join_lines({"A\r\n", "B\r\nB\r", "C\n"}), "A\r\nB\r\nB\rC\n");
    }
    SECTION("容器模式:带line_ends参数") {
        ASSERT_EQ(str::join_lines("\r", {"A", "B", "C"}), "A\rB\rC\r");
        ASSERT_EQ(str::join_lines("\r", std::vector{"A", "B", "C"}), "A\rB\rC\r");
        ASSERT_EQ(str::join_lines("\r", std::list{"A", "B"}), "A\rB\r");
        ASSERT_EQ(str::join_lines("\r", std::array{"A"}), "A\r");
        ASSERT_EQ(str::join_lines("\r", {}), "");
        ASSERT_EQ(str::join_lines("\r", std::list{"A", "", "C"}), "A\r\rC\r");
        ASSERT_EQ(str::join_lines("\r", std::array{"", "", ""}), "\r\r\r");
        ASSERT_EQ(str::join_lines("\r", {"A\r\n", "B\r\nB\r", "C\n"}), "A\r\nB\r\nB\rC\n");
    }
    SECTION("proc模式:不带line_ends参数") {
        ASSERT_EQ(str::join_lines(to_proc{std::initializer_list{"A", "B", "C"}}), "A\nB\nC\n");
        ASSERT_EQ(str::join_lines(to_proc{std::vector{"A", "B", "C"}}), "A\nB\nC\n");
        ASSERT_EQ(str::join_lines(to_proc{std::list{"A", "B"}}), "A\nB\n");
        ASSERT_EQ(str::join_lines(to_proc{std::array{"A"}}), "A\n");
        ASSERT_EQ(str::join_lines(to_proc{std::initializer_list<const char*>{}}), "");
        ASSERT_EQ(str::join_lines(to_proc{std::list{"A", "", "C"}}), "A\n\nC\n");
        ASSERT_EQ(str::join_lines(to_proc{std::array{"", "", ""}}), "\n\n\n");
        ASSERT_EQ(str::join_lines(to_proc{std::initializer_list{"A\r\n", "B\r\nB\r", "C\n"}}), "A\r\nB\r\nB\rC\n");
    }
    SECTION("proc模式:带line_ends参数") {
        ASSERT_EQ(str::join_lines("\r", to_proc{std::initializer_list{"A", "B", "C"}}), "A\rB\rC\r");
        ASSERT_EQ(str::join_lines("\r", to_proc{std::vector{"A", "B", "C"}}), "A\rB\rC\r");
        ASSERT_EQ(str::join_lines("\r", to_proc{std::list{"A", "B"}}), "A\rB\r");
        ASSERT_EQ(str::join_lines("\r", to_proc{std::array{"A"}}), "A\r");
        ASSERT_EQ(str::join_lines("\r", to_proc{std::initializer_list<const char*>{}}), "");
        ASSERT_EQ(str::join_lines("\r", to_proc{std::list{"A", "", "C"}}), "A\r\rC\r");
        ASSERT_EQ(str::join_lines("\r", to_proc{std::array{"", "", ""}}), "\r\r\r");
        ASSERT_EQ(str::join_lines("\r", to_proc{std::initializer_list{"A\r\n", "B\r\nB\r", "C\n"}}), "A\r\nB\r\nB\rC\n");
    }
}
