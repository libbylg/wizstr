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

#include <array>
#include <list>
#include <vector>

TEST(test_str, join) {
    SECTION("一般场景") {
        ASSERT_EQ(str::join(",", {"A", "B", "C"}), "A,B,C");
        ASSERT_EQ(str::join(",", std::vector{"A", "B", "C"}), "A,B,C");
        ASSERT_EQ(str::join(";", std::vector{"A", "B"}), "A;B");
        ASSERT_EQ(str::join("#", std::vector{"A"}), "A");
        ASSERT_EQ(str::join("|", std::vector{"A", "B", "C"}), "A|B|C");
        ASSERT_EQ(str::join("", std::vector{"A", "B", "C"}), "ABC");
        ASSERT_EQ(str::join("|", std::vector{"A", "", "C"}), "A||C");
        ASSERT_EQ(str::join("/", std::list{"A", "B", "C"}), "A/B/C");
        ASSERT_EQ(str::join("*", std::array{"A", "B", "C"}), "A*B*C");
        ASSERT_EQ(str::join("*", std::array{"", "", ""}), "**");
    }
    SECTION("使用 initialize_list 形式") {
        ASSERT_EQ(str::join(",", {"A", "B", "C"}), "A,B,C");
        ASSERT_EQ(str::join(";", {"A", "B"}), "A;B");
    }
    SECTION("proc使用供给数据:一般情况") {
        std::array items{"A", "B", "C"};
        size_t i = 0;
        ASSERT_TRUE(str::join(",", [&i, &items]() -> std::optional<std::string_view> {
            if (i >= items.size()) {
                return std::nullopt;
            }

            return items[i++];
        }) == "A,B,C");
    }
    SECTION("proc使用供给数据:总是返回nullopt") {
        ASSERT_TRUE(str::join(",", []() -> std::optional<std::string_view> {
            return std::nullopt;
        }) == "");
    }
    SECTION("分隔符为空") {
        ASSERT_EQ(str::join("", {"A", "B", "C"}), "ABC");
    }
    SECTION("空序列") {
        ASSERT_EQ(str::join("|", std::vector<str::const_pointer>{}), "");
    }
}
