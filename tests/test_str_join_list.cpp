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

#include <array>
#include <list>
#include <vector>

#if defined(STR_NAMESPACE)
using str = STR_NAMESPACE::str;
#endif

TEST(test_str, join_list) {
    SECTION("一般场景") {
        ASSERT_EQ(str::join_list({"A", "B", "C"}), "A,B,C");
        ASSERT_EQ(str::join_list(std::vector{"A", "B", "C"}), "A,B,C");
        ASSERT_EQ(str::join_list(std::vector{"A", "B"}), "A,B");
        ASSERT_EQ(str::join_list(std::vector{"A"}), "A");
        ASSERT_EQ(str::join_list(std::list{"A", "", "C"}), "A,,C");
        ASSERT_EQ(str::join_list(std::array{"", "", ""}), ",,");
    }
    SECTION("使用 initialize_list 形式") {
        ASSERT_EQ(str::join_list({"A", "B", "C"}), "A,B,C");
        ASSERT_EQ(str::join_list({"A", "B"}), "A,B");
    }
    SECTION("proc使用供给数据:一般情况") {
        std::array items{"A", "B", "C"};
        size_t i = 0;
        ASSERT_TRUE(str::join_list([&i, &items]() -> std::optional<std::string_view> {
            if (i >= items.size()) {
                return std::nullopt;
            }

            return items[i++];
        }) == "A,B,C");
    }
    SECTION("proc使用供给数据:总是返回nullopt") {
        ASSERT_TRUE(str::join_list([]() -> std::optional<std::string_view> {
            return std::nullopt;
        }) == "");
    }
    SECTION("空序列") {
        ASSERT_EQ(str::join_list(std::vector<str::const_pointer>{}), "");
    }
}
